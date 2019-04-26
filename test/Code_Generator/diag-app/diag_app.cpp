#define LOG_TAG "[DIAGMGR_APP]"

#include <Log.h>

#include <stdio.h>
#include <utils/Looper.h>

#include "diag_app.h"
#include "HMIType.h"

static android::sp<Application> gApp;

diagSampleApplication::diagSampleApplication() {
    
}

diagSampleApplication::~diagSampleApplication() {
}

void diagSampleApplication::onCreate() {
    
    LOGV("diagSampleApplication onCreate()");
    mAppManager = interface_cast<IApplicationManagerService>(defaultServiceManager()->getService(String16(APPLICATION_SRV_NAME)));
    mSystemReceiver = new SystemPostReceiver(*this);
    mAppManager->registerSystemPostReceiver(mSystemReceiver, SYS_POST_BOOT_COMPLETED/* system post */);

    mdiagHandler = new diagHandler(*this);

    mDiagManagerService = interface_cast<IDiagManagerService>(defaultServiceManager()->getService(String16(DIAG_SRV_NAME)));
    mDiagReceiver = new diagSampleReceiver(*this);

    if ((mDiagManagerService != NULL) && (mDiagReceiver != NULL)) {
    	
                mDiagManagerService->attachReceiver(DID_R_Firewall_error_detection_times, mDiagReceiver); 
                mDiagManagerService->attachReceiver(DID_W_Firewall_error_detection_times, mDiagReceiver); 
    }
    else 
        LOGE("mDiagManagerService or  mDiagReceiver is NULL");
    
    initTimer();
}



void diagSampleApplication::onDestroy() {
    LOGV("onDestroy()..");
}


bool diagSampleApplication::onSystemPostReceived(const sp<Post>& systemPost)
{
    LOGV("onSystemPostReceived(), what = %d", systemPost->what);
    return true;
}

void diagSampleApplication::test1()
{
    if(mDiagManagerService == NULL)
    {
     //   mDiagManagerService = interface_cast<IdiagManagerService> (defaultServiceManager()->getService(String16(diag_SRV_NAME)));
    }

    if(mDiagManagerService==NULL)
    {
        LOGV("test1(). couldn't get diagmanager Service");
        return;
    }

}

void diagSampleApplication::test2()
{

    if(mDiagManagerService==NULL)
    {
        LOGV("test2() . couldn't get diagationManager Service");
        return;
    }
}

void diagSampleApplication::test3()
{

    if(mDiagManagerService==NULL)
    {
        LOGV("test3() . couldn't get diagationManager Service");
        return;
    }
}

void diagSampleApplication::initTimer()
{

    mTimerImp = new TimeoutHandlerImp (*this);
    mPosTimer = new Timer (mTimerImp, TIMER_DIAG_TIME);
    mPosTimer->setDuration(10,5);
    mPosTimer->start();
}

/**
* Write your handler code below!
*/

void diagSampleApplication::TimeoutHandlerImp::handlerFunction (int timer_id)
{
    LOGV("TimerHandlerFunction id: %d",timer_id);
    switch (timer_id)
    {

    /*
        case TIMER_DIAG_TIME:
			timer_count++;
           if ( timer_count % 2)
              mApp.mDiagManagerService->registerReceiver();
           else
              mApp.mDiagManagerService->unregisterReceiver();
        break;
        default:
        break;
    */

    }

    
}
    

void diagSampleApplication::onRcvDiagInfo(uint16_t id, sp<Buffer>& buf)
{
            if(id == DID_R_Firewall_error_detection_times)  //read data id
            { // 
                LOGV("diagSampleApplication DID_R_Firewall_error_detection_times");

                 int size = 6;
                 uint8_t * temp = new uint8_t[size];
            
                temp[0] = 0x11;
                temp[1] = 0x22;
                temp[2] = 0x33;
                temp[3] = 0x44;
                temp[4] = 0x55;
                temp[5] = 0x66;

                sp<Buffer> tempRbuf = new Buffer();
                tempRbuf->setSize(size);
                tempRbuf->setTo(temp,size);

                if(mDiagManagerService !=NULL)
                    mDiagManagerService->readDataID((uint16_t)id,tempRbuf);

            }
            else if (id == DID_W_Firewall_error_detection_times) {          // write data id

            LOGV("diagSampleApplication DID_W_Firewall_error_detection_times");
                mDiagManagerService->writeDataID(id,buf);       
            }
}


void diagSampleApplication::diagHandler::handleMessage(const sp<sl::Message>& msg) {
   
    sp<Buffer> buf = new Buffer;
    if (msg->buffer.size() > 0) {
        buf->setTo(msg->buffer.data(), msg->buffer.size());
    }



    switch(msg->what) {

      
    case MSG_SEND_0:
        LOGV("MSG_SEND_0 received");
        mApp.test1();
        obtainMessage(diagHandler::MSG_SEND_1)->sendToTarget();
        break;

    case MSG_SEND_1:
        LOGV("MSG_SEND_1 received ");
        mApp.test2();
        obtainMessage(diagHandler::MSG_SEND_2)->sendToTarget();
        break;

    case MSG_SEND_2:
        mApp.test3();
        LOGV("MSG_SEND_2 received ");
        break;

    case MSG_FROM_DIAG:
        mApp.onRcvDiagInfo((uint16_t)msg->arg1, buf);
        LOGV("MSG_SEND_3 received ");
        break;

        
        default:
        break;
      
    }

 }


#ifdef __cplusplus
extern "C" class Application* createApplication() {
    printf("create diagSampleApplication");
    gApp = new diagSampleApplication;
    return gApp.get();
}

extern "C" void destroyApplication(class Application* application) {
    delete (diagSampleApplication*)application;
}
#endif
