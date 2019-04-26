# Flow Diagram

```puml

class BpDiagManagerService
class BnDiagManagerService
class IDiagManagerService
class android::IInterface
class ServiceStub
class ReceiverDeathRecipient

 IDiagManagerService -up-> android::IInterface
 BpDiagManagerService -up->  IDiagManagerService
 BnDiagManagerService -up->  IDiagManagerService
 ServiceStub -up-> BnDiagManagerService
 DiagManagerService -up-> SystemService
 ServiceStub --* DiagManagerService : diag contains stub <
 ReceiverDeathRecipient --* DiagManagerService : diag contains Death <

```



```puml
IDiagManagerService -> BpDiagManagerService : functionA()
BpDiagManagerService ->  binder : <functionA>\n Parcel data
BpDiagManagerService ->  binder : <functionA>\n remote->transact(CODE,Parcel data)
binder -> BnDiagManagerService :
BnDiagManagerService -> ServiceStub : onTransact(CODE,Parcel& data,\n Parcel* reply,flag)\n swich(CODE){ functionA(); }
ServiceStub -> DiagManagerService : parent.functionA()
note right DiagManagerService : functionA()

```

- characteristics of this binder
  - all is synchronization.
- what is shortage of current architecture?
  - class DiagData의 data 구조 변경 가능하게
    - writeToParcel도 이 구조에 맞춰서 자동으로 만들어지게 하면 좋음.
  - class DiagManagerService 안에서 compound로 class ServiceStub:BnDiagManagerService를 가지지 않아도 되는 구조로..
  - serviceid_t를 Print할때 string으로 찍어주게 한다.
  - ServiceStub member에서 return해도 되는 것인가?
  - IDiagData.cpp : 중복되는 코드는 없는가?  더 간략히 표현할 곳은 없는가? mLen==0이면 size로 2개로 하는 이유?



- class DiagManagerService 의 onInit() , onStart() 에서 시작한다.
- SystemService::onStart()를 수행하고 , 여기서부터 event를 처리한다.

## analysis of DiagInputManager
- 이것은 DiagService가  Client로 동작하는 과정이다.  vif , audio service에 등록하여 정보를 받게 하는 부분이다.
- 각 service (app , vif , audio , location etc)에 등록하기 위해서 , Receiver / Manager를 받는다.

## 추가적으로 구현해야 하는 내용
- Manager에서 Service가 죽었다 살아났을때 register를 다시 해주는 부분을 만들어야 한다.

```puml
class DiagInputManager
class DiagHandler
class sl::Handler
class android::RefBase
class DiagInputMgrTimer
class TimerTimeoutHandler
class ServiceDeathRecipient
class android::IBinder::DeathRecipient
class DiagSystemPostReceiver
class BnSystemPostReceiver
class audioReceiver
class BnAudioReceiver
class hmiReceiver
class BnHMIEventReceiver
class vifReceiver
class BnvifManagerReceiver
class DiagManagerService

DiagInputManager *-up-  DiagManagerService :  >
 DiagInputManager -up-> android::RefBase
 DiagInputManager *-up-  DiagHandler : mMyHandler >
 DiagHandler -up->  sl::Handler
 DiagInputManager *-up-  DiagInputMgrTimer : mDiagInputMgrTimer >
 DiagInputMgrTimer -up-> TimerTimeoutHandler
 DiagInputManager *-up-  ServiceDeathRecipient : mServiceDeathRecipient >
 ServiceDeathRecipient -up->  android::IBinder::DeathRecipient
 DiagInputManager *-up-  DiagSystemPostReceiver : mSystemPostReceiver >
 DiagSystemPostReceiver -up-> BnSystemPostReceiver : appmgr
 DiagInputManager *-up-  audioReceiver : mAudioReceiver >
 audioReceiver -up-> BnAudioReceiver : audio
 DiagInputManager *-up-  hmiReceiver : mHmiReceiver >
 hmiReceiver -up-> BnHMIEventReceiver : HMI
 DiagInputManager *-up-  vifReceiver : mVifReceiver >
 vifReceiver -up-> BnvifManagerReceiver : vif

```


```puml




class DiagInputManager {
	+DiagInputManager(android::sp<DiagManagerService> diagMgrService)
	+~DiagInputManager()
	-mDiagInputMgrTimer : DiagInputMgrTimer*
	-m_WatcdogTimer : Timer*
	-mMyHandler : android::sp<DiagHandler>
	-mDiagMgrService : android::sp<DiagManagerService>
	-mAppManager : android::sp<IApplicationManagerService>
	-mAudioManager : android::sp<IAudioManagerService>
	-mAudioReceiver : android::sp<IAudioReceiver>
	-mHmiReceiver : android::sp<IHMIEventReceiver>
	-mHmiManager : android::sp<IHMIManagerService>
	-mSystemPostReceiver : android::sp<ISystemPostReceiver>
	-mVifReceiver : android::sp<IvifManagerReceiver>
	-mVifManager : android::sp<IvifManagerService>
	-mServiceDeathRecipient : android::sp<ServiceDeathRecipient>
	+FUNC_DID_WORK_FOR_DEFINE_0(uint8_t* data) : error_t
	+FUNC_DID_WORK_FOR_DEFINE_1(uint8_t* data) : error_t
	+FUNC_DID_WORK_FOR_DEFINE_2(uint8_t* data) : error_t
	+FUNC_DID_WORK_FOR_DEFINE_3(uint8_t* data) : error_t
	+FUNC_DID_WORK_FOR_DEFINE_4(uint8_t* data) : error_t
	+FUNC_DID_WORK_FOR_DEFINE_5(uint8_t* data) : error_t
	+FUNC_DID_WORK_FOR_DEFINE_6(uint8_t* data) : error_t
	+FUNC_DID_WORK_FOR_DEFINE_7(uint8_t* data) : error_t
	+FUNC_DID_WORK_FOR_DEFINE_8(uint8_t* data) : error_t
	+FUNC_DID_WORK_FOR_DEFINE_9(uint8_t* data) : error_t
	+init() : error_t
	+messagefromVIF(uint16_t sigId, android::sp<Buffer>& buf) : error_t
	-connectToAppMgr(void) : void
	-connectToAudioMgr(void) : void
	-connectToHMIMgr(void) : void
	-connectTovifMgr(void) : void
	+onServiceBinderDied(const android::wp<android::IBinder>& who) : void
}


	class DiagInputManager::DiagHandler {
		+DiagHandler(android::sp<sl::SLLooper>& looper, DiagInputManager& outer)
		-mDiagInputMgr : DiagInputManager&
		+handleMessage(const android::sp<sl::Message>& msg) : void
	}


	class DiagInputManager::DiagInputMgrTimer {
		+DiagInputMgrTimer(android::sp<DiagInputManager::DiagHandler> handler)
		-mHandler : android::sp<DiagInputManager::DiagHandler>
		+{static} DIAG_WATCHDOG_TIMER : static const uint32_t
		+handlerFunction(int timer_id) : void
	}


	class DiagInputManager::DiagSystemPostReceiver {
		+DiagSystemPostReceiver(android::sp<DiagInputManager::DiagHandler> handler)
		-mHandler : android::sp<DiagInputManager::DiagHandler>
		+onSystemPostReceived(const android::sp<Post>& systemPost) : bool
	}

	class DiagInputManager::ServiceDeathRecipient {
		+ServiceDeathRecipient(DiagInputManager& parent)
		+~ServiceDeathRecipient()
		-mParent : DiagInputManager&
		+binderDied(const android::wp<android::IBinder>& who) : void
	}


	class DiagInputManager::audioReceiver {
		+audioReceiver(android::sp<DiagInputManager::DiagHandler> handler)
		-mHandler : android::sp<DiagInputManager::DiagHandler>
		+onAudioPlayStateChanged(uint8_t dtc_type) : void
	}


	class DiagInputManager::hmiReceiver {
		+hmiReceiver(android::sp<DiagInputManager::DiagHandler> handler)
		-mHandler : android::sp<DiagInputManager::DiagHandler>
		+onHMIReceived(uint32_t hmi_type, uint32_t hmi_action) : void
	}


	class DiagInputManager::vifReceiver {
		+vifReceiver(android::sp<DiagInputManager::DiagHandler> handler)
		-mHandler : android::sp<DiagInputManager::DiagHandler>
		+onReceive(uint32_t Sigid, android::sp<Buffer>& buf) : void
	}


enum empty {
	MSG_BOOT_COMPLETE_DID_START
	MSG_CONNECT_TO_APPMGR
	MSG_CONNECT_TO_AUDIOMGR
	MSG_CONNECT_TO_HMIMGR
	MSG_CONNECT_TO_VIFMGR
	MSG_DID_WORK_FOR_DEFINE_0
	MSG_DID_WORK_FOR_DEFINE_1
	MSG_DID_WORK_FOR_DEFINE_2
	MSG_DID_WORK_FOR_DEFINE_3
	MSG_DID_WORK_FOR_DEFINE_4
	MSG_DID_WORK_FOR_DEFINE_5
	MSG_DID_WORK_FOR_DEFINE_6
	MSG_DID_WORK_FOR_DEFINE_7
	MSG_DID_WORK_FOR_DEFINE_8
	MSG_DID_WORK_FOR_DEFINE_9
	MSG_RECEIVE_BOOT_COMPLETE
	MSG_RECEIVE_FROM_AUDIO
	MSG_RECEIVE_FROM_HMI
	MSG_RECEIVE_FROM_VIF
	MSG_RECEIVE_WATCH_DOG
	MSG_SIGNAL_INTERNAL_IGNITION2_STATUS
	TIME_SEND_RETRY_DELAY_MS
}




DiagInputManager::DiagHandler o-- .DiagInputManager


DiagInputManager::DiagInputMgrTimer o-- .DiagInputManager


	DiagInputManager::DiagInputMgrTimer o-- DiagInputManager::DiagHandler


DiagInputManager::DiagSystemPostReceiver o-- .DiagInputManager


	DiagInputManager::DiagSystemPostReceiver o-- DiagInputManager::DiagHandler


DiagInputManager::ServiceDeathRecipient o-- .DiagInputManager


DiagInputManager::audioReceiver o-- .DiagInputManager


	DiagInputManager::audioReceiver o-- DiagInputManager::DiagHandler


DiagInputManager::hmiReceiver o-- .DiagInputManager


	DiagInputManager::hmiReceiver o-- DiagInputManager::DiagHandler


DiagInputManager::vifReceiver o-- .DiagInputManager


	DiagInputManager::vifReceiver o-- DiagInputManager::DiagHandler



```




```puml
DiagManagerService -> DiagInputManager : init()
DiagManagerService -> DiagInputManager : onStart()
DiagInputManager -> ServiceDeathRecipient : mServiceDeathRecipient - contructor Death
DiagInputManager -> DiagHandler : mMyHandler - looper of MSG
loop each XXX Module (app , audio , HMI , vif Manager) until all Modules are alive
  DiagInputManager -> DiagInputManager : connectToXXXMgr()
  alt XXX manager is alive
    DiagInputManager <-> Binder : register Death Recipient / mXXXManager
  else
    DiagInputManager -> DiagHandler : handleMessage - MSG_CONNECT_TO_XXXMGR
  end
end
DiagInputManager -> DiagInputMgrTimer : mDiagInputMgrTimer - timer
DiagInputManager -> DiagInputMgrTimer : watchDog start
OtherService -> DiagHandler : handleMessage - DID_WORK_FOR_DEFINE_?
alt die related service module
  binder -> DiagInputManager : onServiceBinderDied
  loop Died Module (app , audio , HMI , vif Manager) until Died Module is alive
    DiagInputManager -> DiagInputManager : connectToXXXMgr()
    alt XXX manager is alive
      DiagInputManager <-> Binder : register Death Recipient / mXXXManager
    else
      DiagInputManager -> DiagHandler : handleMessage - MSG_CONNECT_TO_XXXMGR
    end
  end
end

```


- Biner Basic Operations
```puml
box "Application (Proxy)"
participant App
participant BpDiagManagerService
end box
participant Binder
box "Service (Native)"
participant BnDiagManagerService
participant ServiceStub
end box

App -> BpDiagManagerService : wishtoUseAPI_1(DiagData)
BpDiagManagerService -> Binder : DiagData >> remote()->onTransact(OP_REGISTER_WISHTOUSEAPI_1, Parcel)
Binder -> BnDiagManagerService : onTransact(uint32_t code, const Parcel& data,reply)\n OP_REGISTER_WISHTOUSEAPI_1, Parcel >> DiagData
BnDiagManagerService -> ServiceStub : wishtoUseAPI_2(DiagData)
activate ServiceStub
BnDiagManagerService <- ServiceStub : return E_OK
deactivate ServiceStub
Binder <- BnDiagManagerService : reply->writeInt32
BpDiagManagerService <- Binder : reply.readInt32()
App <- BpDiagManagerService : return
```








# Power service analysis
- send to power manager
    - input_event()  : OP_INPUT_EVENT
    - sldd command
- have an wrapper (Manager)
    - do not need it now. because we send the message without any changes.
- service (stub)
    - application or other service call stub function of BnInterface.
    - IXXXManagerServiceI.cpp <interface>
        - class BpXXXManagerService [app or other service]
            - all functions with the same of BnXXXManagerService
            - it call remote_transact(ID,parcel) after making parcel from arguments.
        - class BnXXXManagerService [XXX service]
            - onTransact(ID , parcel) -> case -> call the same funciton() according to ID
                - OP_INPUT_EVENT : input_event()  -> handler()->sendMessage(msg<PS_REQ_PROCESS_EVENT_INPUTTED>)
- service handler (stub:service)
    - looper : processPowerManager() -> case (what) PS_REQ_PROCESS_EVENT_INPUTTED   -> processing!!!
- receiver (anyone can be receiver)
    - this is started from instance()->registerXXXReceiver(this)
        - in case of power, power service calls Bp function() when power service get message in input_event().
        - Bp function in XXX service calls Bn fuction in registed app or other service.
    - otherService : their own API  in XXXManagerService.h
        - application manager : onSystemPostReceived
    - IXXXReveiver.cpp .h
        - class BpXXXManagerService [XXX service]
            - all functions with the same of BnXXXManagerService
            - it call remote_transact(ID,parcel) after making parcel from arguments.
        - class BnXXXManagerReceiver: [app or other service]
            - onTransact(ID , parcel) -> case -> call the same funciton() according to ID









- Register receiver and call receiver function
```puml
box "Application"
participant DiagSampleApplication
participant BnDiagManagerReceiver
participant BpDiagManagerService
end box
participant Binder
box "DiagService"
participant BpDiagManagerReceiver
participant DiagManagerService
participant ServiceStub
participant BnDiagManagerService
participant DiagInputManager
participant DiagHandler
end box
box "OtherService"
participant mAppManager
participant mVifManager
participant mTargetManager
end box

== Initialization ==
-> DiagManagerService : onInit()
note right DiagManagerService : mReceiverDeathRecipient = new ReceiverDeathRecipient(*this);\n	mDiagMgrHandler = new DiagMgrHandler(looper(), *this);
-> DiagManagerService : onStart()
note right DiagManagerService : mDiagInputMgr = new DiagInputManager(this)
DiagManagerService -> DiagInputManager : mDiagInputMgr->init()
note right DiagInputManager : mMyHandler = new DiagHandler(mDiagMgrService->looper(), *this);\n mDiagInputMgrTimer = new DiagInputMgrTimer (mMyHandler);
loop each Target Module (app , audio , HMI , vif Manager) until all Modules are alive
  DiagInputManager -> DiagInputManager : connectToTargetMgr()
  alt Target manager is alive
    DiagInputManager -> Binder : register receiver to mTargetManager (ex. mAudioReceiver = DiagAudioReceiver: public BnAudioReceiver)
    Binder -> mTargetManager : mTargetManager->registerReceiver(mTargetReceiver, 0);
  else Target manager is not alive
    DiagInputManager -> DiagHandler : handleMessage(MSG_CONNECT_TO_XXXMGR)
    DiagInputManager <- DiagHandler : connectToTargetMgr()
  end
end

== Vif example : Diag registers to vif and receives the msg from vif ==
alt ex. VifManager is alive
  DiagInputManager -> Binder : register mVifReceiver  to mVifManager\n (ex. mVifReceiver = class VifReceiver: public BnvifManagerReceiver)
  Binder -> mVifManager : mVifManager->registerReceiver(Sigid, vif_received_always, mVifReceiver);
  alt Vif send msg to Diag
    mVifManager -> DiagInputManager : mVifManager->onReceive(uint32_t Sigid, android::sp<Buffer>& buf)
    note left DiagInputManager : message->sendToTarget(ObtainMessage());
    DiagInputManager -> DiagHandler : handleMessage(msg)
    note right DiagHandler : switch DIAG_RECEIVE_FROM_VIF
    DiagHandler -> DiagInputManager : messagefromVIF(msg)
    note right DiagInputManager : transferDatabyVIF()
    DiagInputManager -> ServiceStub : queryReceiverById()
    note right ServiceStub : mReceivers[id].onReceive(diagData);
  end
end

== 1. app register receiver to DiagService.  2. DiagService transfer the msg from vif to app ==
alt DiagSampleApplication register to Diag and VIF sends msg to DiagSampleApplication
    power_app -> mAppManager : registerSystemPostReceiver()
    power_app -> powerService : registerReceiver()
end



alt DiagSampleApplication register to Diag and VIF sends msg to DiagSampleApplication
  DiagSampleApplication -> BpDiagManagerService : attachReceiver(id,receiver)
  BpDiagManagerService -> Binder : receiver >> remote()->onTransact(OP_REGISTER_RECEIVER, Parcel)
  Binder -> BnDiagManagerService : onTransact(uint32_t code, const Parcel& data,reply)\n OP_REGISTER_RECEIVER, Parcel >> receiver
  BnDiagManagerService -> ServiceStub : attachReceiver(id,receiver)
  note right ServiceStub : mReceivers[id].push_back(receiver)\nlinkToDeath() : onReceiverBinderDied(app이 죽었으면 mReceivers에서 삭제)
  activate ServiceStub
  BnDiagManagerService <- ServiceStub : return E_OK
  deactivate ServiceStub
  Binder <- BnDiagManagerService : reply->writeInt32
  BpDiagManagerService <- Binder : reply.readInt32()
  DiagSampleApplication <- BpDiagManagerService : return E_OK
  mVifManager -> DiagInputManager : mVifManager->onReceive(uint32_t Sigid, android::sp<Buffer>& buf)
  note left DiagInputManager : message->sendToTarget(obtainMessage());
  DiagInputManager -> DiagHandler : handleMessage(msg)
  note right DiagHandler : switch DIAG_RECEIVE_FROM_VIF
  DiagHandler -> DiagInputManager : messagefromVIF(msg)
  note right DiagInputManager : transferDatabyVIF()
  DiagInputManager -> ServiceStub : queryReceiverById()
  note right ServiceStub : mReceivers[id].onReceive(diagData);
  ServiceStub -> BpDiagManagerReceiver : onReceive(diagData)
  BpDiagManagerReceiver -> Binder : (void)remote()->transact(TRANSACT_ONRECEIVE, data, &reply);
  Binder -> BnDiagManagerReceiver : onTransact(TRANSACT_ONRECEIVE)
  BnDiagManagerReceiver -> DiagSampleApplication : diagSampleReceiver::onReceive()
  note right DiagSampleApplication : message->sendToTarget(obtainMessage());
  DiagSampleApplication -> DiagSampleApplication : diagSampleApplication::diagHandler::handleMessage(msg)
end

== Call DiagService function (attachReceiver) through binder ==
DiagSampleApplication -> mAppManager : registerSystemPostReceiver(mSystemReceiver, SYS_POST_BOOT_COMPLETED)
|||
note right DiagSampleApplication : mAppManager(appmgr),mSystemReceiver(appmgr:BnSystemPostReceiver)\n,mdiagHandler\n,mDiagManagerService(diag),mDiagReceiver(diag:BnDiagManagerReceiver) 이름통일!!\n interface_cast<IDiagManagerService>(defaultServiceManager()->getService(String16(DIAG_SRV_NAME)))에서 interface_cast가 Bp로 변경을 시켜줌
DiagSampleApplication -> BpDiagManagerService : attachReceiver(DID_W_Firewall_error_detection_times, mDiagReceiver) 순서!!
|||
DiagSampleApplication -> BpDiagManagerService : attachReceiver(id,receiver)
BpDiagManagerService -> Binder : receiver >> remote()->onTransact(OP_REGISTER_RECEIVER, Parcel)
Binder -> BnDiagManagerService : onTransact(uint32_t code, const Parcel& data,reply)\n OP_REGISTER_RECEIVER, Parcel >> receiver
BnDiagManagerService -> ServiceStub : attachReceiver(id,receiver)
note right ServiceStub : mReceivers[id].push_back(receiver)\nlinkToDeath() : onReceiverBinderDied(app이 죽었으면 mReceivers에서 삭제)
activate ServiceStub
BnDiagManagerService <- ServiceStub : return E_OK
deactivate ServiceStub
Binder <- BnDiagManagerService : reply->writeInt32
BpDiagManagerService <- Binder : reply.readInt32()
DiagSampleApplication <- BpDiagManagerService : return E_OK
|||

== Call DiagService function (wishtoUseAPI_1) through binder ==
DiagSampleApplication -> BpDiagManagerService : wishtoUseAPI_1(DiagData)
BpDiagManagerService -> Binder : DiagData >> remote()->onTransact(OP_REGISTER_WISHTOUSEAPI_1, Parcel)
Binder -> BnDiagManagerService : onTransact(uint32_t code, const Parcel& data,reply)\n OP_REGISTER_WISHTOUSEAPI_1, Parcel >> DiagData
BnDiagManagerService -> ServiceStub : wishtoUseAPI_2(DiagData)
activate ServiceStub
BnDiagManagerService <- ServiceStub : return E_OK
deactivate ServiceStub
Binder <- BnDiagManagerService : reply->writeInt32
BpDiagManagerService <- Binder : reply.readInt32()
DiagSampleApplication <- BpDiagManagerService : return
|||

```
