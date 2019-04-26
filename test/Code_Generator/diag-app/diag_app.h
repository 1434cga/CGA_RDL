#ifndef _DIAG_APPLICATION_H_
#define _DIAG_APPLICATION_H_


#include <utils/SLLooper.h>
#include <utils/Handler.h>
#include <utils/Timer.h>
#include <binder/IServiceManager.h>

#include <corebase/application/Application.h>
#include <services/ApplicationManagerService/IApplicationManagerService.h>
#include <services/ApplicationManagerService/IApplicationManagerServiceType.h>

#include <services/vifManagerService/IvifManagerServiceType.h>
#include <services/vifManagerService/IvifManagerService.h>
#include <services/vifManagerService/IvifManagerReceiver.h>

//#include "services/DiagManagerService/DiagCommand.h"
//#include "services/DiagManagerService/DiagManagerService.h"

#include <services/DiagManagerService/IDiagManagerService.h>
#include <services/DiagManagerService/IDiagManagerReceiver.h>


#define TIMER_DIAG_TIME        (0x0001)
#define DID_R_Firewall_error_detection_times   0x7001
#define DID_W_Firewall_error_detection_times   0x8001


class diagSampleApplication : public Application 
{

        private:
            class diagHandler : public Handler {
                public:
                    static const uint32_t MSG_SEND_0 = 0;
                    static const uint32_t MSG_SEND_1 = 1;
                    static const uint32_t MSG_SEND_2 = 2;
                    static const uint32_t MSG_FROM_DIAG = 3;

                    diagHandler(diagSampleApplication &app) : mApp(app) {}
                    virtual ~diagHandler() {}
                    virtual void handleMessage(const sp<sl::Message>& msg);
                private:
                    diagSampleApplication &mApp;
            };

           class TimeoutHandlerImp : public TimerTimeoutHandler
            {
                public:
                    TimeoutHandlerImp (diagSampleApplication &app):mApp(app){};
                    ~TimeoutHandlerImp () {};

                    void handlerFunction (int);
                private:
                    diagSampleApplication &mApp;
        			uint16_t timer_count;
            };

            class  SystemPostReceiver : public BnSystemPostReceiver
            {
                public:
                        SystemPostReceiver(diagSampleApplication& app) : mApp(app) {}
                        virtual bool onSystemPostReceived(const sp<Post>& systemPost)
                        {
                                return mApp.onSystemPostReceived(systemPost);
                        }
                        
                private:
                        diagSampleApplication &mApp;
            };

             class diagSampleReceiver : public BnDiagManagerReceiver 
            {
                 public:
                        diagSampleReceiver(diagSampleApplication& app) : mApp(app) {}
                        virtual ~diagSampleReceiver(){}
                        virtual void onReceive(int32_t id, uint8_t type, uint8_t cmd, uint8_t cmd2, sp<Buffer>& buf)
                        {
                             //     SYS_LOGD("diagSampleReceiver onReceive");
                             //     SYS_LOGD("diagSampleReceiver signal id  %d",id);

                                sp<sl::Message> message = mApp.mdiagHandler->obtainMessage(diagHandler::MSG_FROM_DIAG, id);
                             
                                if (buf->size() > 0) {
                                       message->buffer.setTo(buf->data(), buf->size());
                                } else {
                                    LOGE("Diag signal received without data.");
                                }
                                message->sendToTarget();
                      //          mApp.onRcvDiagInfo((uint16_t)id, buf);
                        }
                        virtual void onDiagcmd(int32_t id, uint8_t type, uint8_t cmd, uint8_t cmd2, sp<Buffer>& buf){};

                    private :
                            diagSampleApplication &mApp;

            };


            bool onSystemPostReceived(const sp<Post>& systemPost);

    /**
    * Timer
    */
            diagSampleApplication::TimeoutHandlerImp* mTimerImp;
            Timer* mPosTimer;


    /**
    * Declare managers to use
    */
            sp<IApplicationManagerService> mAppManager;
            sp<IDiagManagerService> mDiagManagerService;
            sp<IDiagManagerReceiver> mDiagReceiver;
            sp<ISystemPostReceiver> mSystemReceiver;
            sp<diagHandler> mdiagHandler;
             
    public:
    diagSampleApplication();
    virtual ~diagSampleApplication();
    void onRcvDiagInfo(uint16_t id, sp<Buffer>& buf);

    /**
    * Application has two lifecycle method, onCreate() and onDestroy()
    */
    virtual void onCreate();
    virtual void onDestroy();


    /**
    * your own method
    */
    void test1();
    void test2();
    void test3();
    void initTimer();
};


#endif // _DIAG_APPLICATION_H_
