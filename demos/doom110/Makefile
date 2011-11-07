
TARGET	= 0doom

NEWLIB	= ../../../newlib

TOOLS_DIR = ../../bin
LIBGAME	= ../libgame

# RESOBJS	= loading.o

OBJS	= ./objs/g_game.o ./objs/am_map.o ./objs/f_wipe.o  ./objs/m_random.o \
	./objs/p_map.o ./objs/p_spec.o ./objs/st_stuff.o ./objs/d_items.o ./objs/m_swap.o \
	./objs/p_maputl.o ./objs/p_switch.o ./objs/r_plane.o ./objs/tables.o ./objs/d_main.o ./objs/hu_lib.o \
	./objs/m_argv.o	./objs/p_ceilng.o ./objs/p_mobj.o ./objs/p_telept.o ./objs/r_segs.o ./objs/v_video.o \
	./objs/hu_stuff.o ./objs/m_bbox.o ./objs/p_doors.o ./objs/p_plats.o ./objs/p_tick.o \
	./objs/r_sky.o ./objs/wi_stuff.o ./objs/doomdef.o ./objs/m_cheat.o ./objs/p_enemy.o \
	./objs/s_sound.o ./objs/p_pspr.o ./objs/p_user.o ./objs/r_things.o ./objs/w_wad.o ./objs/doomstat.o \
	./objs/m_fixed.o ./objs/p_floor.o ./objs/p_saveg.o ./objs/r_bsp.o ./objs/sounds.o ./objs/z_zone.o \
	./objs/dstrings.o ./objs/info.o ./objs/m_menu.o ./objs/p_inter.o ./objs/p_setup.o ./objs/r_data.o \
	./objs/f_finale.o ./objs/m_misc.o ./objs/p_lights.o  ./objs/p_sight.o ./objs/i_sound.o \
	./objs/r_draw.o ./objs/st_lib.o ./objs/i_system.o ./objs/r_main.o ./objs/i_video.o ./objs/i_main.o \
	./objs/i_spmp.o ./objs/d_net.o

# OBJS	+= $(RESOBJS)

LIBS	= -lgame -lc -lgcc -lgame

include ../libgame/libgame.mk

CFLAGS += -DSPMP8 -DNO_SOUND
#-DNORMALUNIX

objs/%.o: %.c
		$(CC) $(CFLAGS) $< -o $@

