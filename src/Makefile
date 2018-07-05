CC      = gcc
C_FLAGS = -Wall -ggdb -Dlinux
L_FLAGS = -lcrypt

O_FILES = act_comm.o act_info.o act_move.o act_obj.o act_wiz.o comm.o const.o \
          db.o fight.o handler.o interp.o magic.o save.o special.o smoo.o\
          kav_info.o kav_fight.o kav_wiz.o clan.o  mage.o rot_wiz.o \
          drow.o arena.o ninja.o vamp.o vampie.o ww.o wizutil.o golem.o\
          rotain.o update.o monk.o daemon.o jedi.o signals.o\
	  build.o darkhear.o amazon.o detox.o other.o shape.o hotboot.o\
	  class.o uni.o bere.o newfight.o moogle.o ninjacomb.o objec.o\
	  king.o druid.o fluff.o change.o gamble.o

merc: $(O_FILES)
	rm -f merc
	$(CC) $(L_FLAGS) -o merc $(O_FILES)

#Append merc.h to the next line if your not that good of a coder (safety)
.c.o:
	$(CC) -c $(C_FLAGS) $<

link:
	rm -f merc
	$(CC) $(L_FLAGS) -o merc $(O_FILES)


