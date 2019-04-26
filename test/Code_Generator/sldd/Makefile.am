# Makefile.am - make executable for service layer process

bin_PROGRAMS = sldd

sldd_SOURCES = \
	sldd.cpp \
	man.cpp \
	cansignal.cpp \
	AlarmManager.cpp \
	AppManager.cpp \
	AudioManager.cpp \
	CommunicationManager.cpp \
	ConfigurationManager.cpp \
	LocationManager.cpp \
	PowerManager.cpp \
	SystemManager.cpp \
	WatchdogManager.cpp \
	systemprop.cpp

telephony_sources = \
	telephony/iccUtils.cpp \
	telephony/gsmAlphabet.cpp \
	telephony/telephony.cpp \
	telephony/sms.cpp \
	telephony/data.cpp

sldd_SOURCES += \
	$(telephony_sources)

sldd_CPPFLAGS := \
	-I$(top_srcdir) \
	-I$(WORKSPACE)/lge/service_layer \
	-I$(WORKSPACE)/lge/service_layer/include \
	-include $(WORKSPACE)/system/core/include/arch/linux-arm/OEConfig.h \
    -I$(WORKSPACE)/lge/utils/systemproperty/include \
	-include $(top_builddir)/config.h \
	-Wall \
	-Wno-unused-parameter \
	-fpermissive \
	$(AM_CPPFLAGS)

sldd_LDFLAGS = \
	-lpthread \
	-lrt \
	-lbinder \
	-lcutils \
	-llog \
	-lautils \
	-lcorebase \
	-lpost \
	-liapplication \
	-lcontext \
	-lserviceutils \
	-lalarmhal \
	-lialarm \
	-lapplication \
	-liaudio \
	-liconfiguration \
	-lilocation \
	-lkeeppower \
	-lipower \
	-lisystem \
	-ltelephonymanager \
	-litelephony \
	-livif \
	-liwatchdog
