dvb-a867-objs := a867_af903x-core.o a867_af903x-devices.o a867_af903x-drv.o \
			a867_af903x-fe.o a867_af903x-tuner.o a867_cmd.o a867_standard.o \
			a867_demodulator.o a867_demodulatorextend.o a867_usb2impl.o \
			a867_user.o a867_mxl5007t.o a867_Maxlinear_MXL5007.o a867_Afa_AF9007.o

obj-$(CONFIG_DVB_A867) += dvb-a867.o

EXTRA_CFLAGS += -Idrivers/media/dvb/dvb-core -Idrivers/media/dvb/dvb-usb -Idrivers/media/dvb/frontends
