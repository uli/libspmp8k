/*
** Kid Vid sample file loading routines
*/


/*
** z26 is Copyright 1997-2011 by John Saeger and contributors.  
** z26 is released subject to the terms and conditions of the 
** GNU General Public License Version 2 (GPL).	z26 comes with no warranty.
** Please see COPYING.TXT for details.
*/

FILE *SampleFile, *SharedSampleFile;
db SampleByte;
int FileOpened=0;
dd kv_TapeBusy;
int kv_FilePointer, kv_SharedData, kv_Beep;
unsigned long int kv_SongCounter=0;


unsigned char SongPositions[44+38+42+62+80+62] = {
/* kvs1 44 */
   11,
   12+0x80,
   13+0x80,
   14,
   15+0x80,
   16,
   8+0x80,
   17,
   18+0x80,
   19,
   20+0x80,
   21,
   8+0x80,
   22,
   15+0x80,
   23,
   18+0x80,
   14,
   20+0x80,
   16,
   18+0x80,
   17,
   15+0x80,
   19,
   8+0x80,
   21,
   20+0x80,
   22,
   18+0x80,
   23,
   15+0x80,
   14,
   20+0x80,
   16,
   8+0x80,
   22,
   15+0x80,
   23,
   18+0x80,
   14,
   20+0x80,
   16,
   8+0x80,
   9,

/* kvs2 38 */
   25+0x80,
   26,
   27,
   28,
   8,
   29,
   30,
   26,
   27,
   28,
   8,
   29,
   30,
   26,
   27,
   28,
   8,
   29,
   30,
   26,
   27,
   28,
   8,
   29,
   30,
   26,
   27,
   28,
   8,
   29,
   30,
   26,
   27,
   28,
   8,
   29,
   30+0x80,
   9,

/* kvs3 42 */
   32,
   33,
   34,
   35,
   36,
   37,
   38,
   39,
   40,
   41,
   34,
   42,
   36,
   43,
   40,
   39,
   38,
   37,
   34,
   43,
   36,
   39,
   40,
   37,
   38,
   43,
   34,
   37,
   36,
   43,
   40,
   39,
   38,
   37,
   34,
   43,
   36,
   39,
   40,
   37,
   38+0x80,
   9,

/* kvb1 62 */
   0,
   1,
   45,
   2,
   3,
   46,
   4,
   5,
   47,
   6,
   7,
   48,
   4,
   3,
   49,
   2,
   1,
   50,
   6,
   7,
   51,
   4,
   5,
   52,
   6,
   1,
   53,
   2,
   7,
   54,
   6,
   5,
   45,
   2,
   1,
   46,
   4,
   3,
   47,
   2,
   5,
   48,
   4,
   7,
   49,
   6,
   1,
   50,
   2,
   5,
   51,
   6,
   3,
   52,
   4,
   7,
   53,
   2,
   1,
   54,
   6+0x80,
   9,

/* kvb2 80 */
   0,
   1,
   56,
   4,
   3,
   57,
   2,
   5,
   58,
   6,
   7,
   59,
   2,
   3,
   60,
   4,
   1,
   61,
   6,
   7,
   62,
   2,
   5,
   63,
   6,
   1,
   64,
   4,
   7,
   65,
   6,
   5,
   66,
   4,
   1,
   67,
   2,
   3,
   68,
   6,
   5,
   69,
   2,
   7,
   70,
   4,
   1,
   71,
   2,
   5,
   72,
   4,
   3,
   73,
   6,
   7,
   74,
   2,
   1,
   75,
   6,
   3,
   76,
   4,
   5,
   77,
   6,
   7,
   78,
   2,
   3,
   79,
   4,
   1,
   80,
   2,
   7,
   81,
   4+0x80,
   9,

/* kvb3 62 */
   0,
   1,
   83,
   2,
   3,
   84,
   4,
   5,
   85,
   6,
   7,
   86,
   4,
   3,
   87,
   2,
   1,
   88,
   6,
   7,
   89,
   2,
   5,
   90,
   6,
   1,
   91,
   4,
   7,
   92,
   6,
   5,
   93,
   4,
   1,
   94,
   2,
   3,
   95,
   6,
   5,
   96,
   2,
   7,
   97,
   4,
   1,
   98,
   6,
   5,
   99,
   4,
   3,
   100,
   2,
   7,
   101,
   4,
   1,
   102,
   2+0x80,
   9
};

unsigned long int SongStart[104] = {
/* kvshared */
   44,          /* Welcome + intro Berenstain Bears */
   980829,      /* boulders in the road */
   1178398,     /* standing ovations */
   1430063,     /* brother bear */
   1691136,     /* good work */
   1841665,     /* crossing a bridge */
   2100386,     /* not bad (applause) */
   2283843,     /* ourgame */
   2629588,     /* start the parade */
   2824805,     /* rewind */
   3059116,

/* kvs1 */
   44,          /* Harmony into 1 */
   164685,      /* falling notes (into 2) */
   395182,      /* instructions */
   750335,      /* high notes are high */
   962016,      /* my hat's off to you */
   1204273,     /* 1 2 3 do re mi */
   1538258,     /* Harmony */
   1801683,     /* concratulations (all of the Smurfs voted) */
   2086276,     /* line or space */
   2399093,     /* hooray */
   2589606,     /* hear yeeh */
   2801287,     /* over the river */
   3111752,     /* musical deduction */
   3436329,

/* kvs2 */
   44,          /* Handy intro + instructions */
   778557,      /* place in shape */
   1100782,     /* sailor mate + whistle */
/*  1281887, */
   1293648,     /* attention */
   1493569,     /* colours */
   1801682,     /* congratulations (Handy and friends voted) */
   2086275,

/* kvs3 */
   44,          /* Greedy and Clumsy intro + instructions */
   686829,      /* red */
   893806,      /* don't count your chicken */
   1143119,     /* yellow */
   1385376,     /* thank you */
   1578241,     /* mixin' and matchin' */
   1942802,     /* fun / colour shake */
   2168595,     /* colours can be usefull */
   2493172,     /* hip hip horay */
   2662517,     /* green */
   3022374,     /* purple */
   3229351,     /* white */
   3720920,

/* kvb1 */
   44,          /* 3 */
   592749,      /* 5 */
   936142,      /* 2 */
   1465343,     /* 4 */
   1787568,     /* 1 */
   2145073,     /* 7 */
   2568434,     /* 9 */
   2822451,     /* 8 */
   3045892,     /* 6 */
   3709157,     /* 0 */
   4219542,

/* kvb2 */
   44,          /* A */
   303453,      /* B */
   703294,      /* C */
   1150175,     /* D */
   1514736,     /* E */
   2208577,     /* F */
   2511986,     /* G */
   2864787,     /* H */
   3306964,     /* I */
   3864389,     /* J */
   4148982,     /* K */
   4499431,     /* L */
   4824008,     /* M */
   5162697,     /* N */
   5581354,     /* O */
   5844779,     /* P */
   6162300,     /* Q */
   6590365,     /* R */
   6839678,     /* S */
   7225407,     /* T */
   7552336,     /* U */
   7867505,     /* V */
   8316738,     /* W */
   8608387,     /* X */
   8940020,     /* Y */
   9274005,     /* Z */
   9593878,

/* kvb3 */
   44,          /* cat */
   341085,      /* one */
   653902,      /* red */
   1018463,     /* two */
   1265424,     /* dog */
   1669969,     /* six */
   1919282,     /* hat */
   2227395,     /* ten */
   2535508,     /* mom */
   3057653,     /* dad */
   3375174,     /* ball */
   3704455,     /* fish */
   4092536,     /* nine */
   4487673,     /* bear */
   5026282,     /* four */
   5416715,     /* bird */
   5670732,     /* tree */
   6225805,     /* rock */
   6736190,     /* book */
   7110159,     /* road */
   7676992
};

void kv_OpenSampleFile(void)
{
char kvNameTable[6][9] =
   {"kvs3.wav", "kvs1.wav", "kvs2.wav", "kvb3.wav", "kvb1.wav", "kvb2.wav"};
int StartSong[6] = {44+38, 0, 44, 44+38+42+62+80, 44+38+42, 44+38+42+62};
int i;
   if(!FileOpened){
      if(KidVid==0x44) i=0;
      else i=3;
      i=i+KidVidTape-1;
      if(KidVidTape==4) i=i-3;

      if(quiet==0){
         SampleFile=fopen(&kvNameTable[i][0], "rb");
         if(SampleFile!=NULL){
            SharedSampleFile=fopen("kvshared.wav", "rb");
            if(SharedSampleFile==NULL){
               fclose(SampleFile);
               FileOpened=0;
            }else{
               FileOpened=1;
               fseek(SampleFile, 45, SEEK_SET);
            }
         }else FileOpened=0;
      }else FileOpened=0;

      kv_SongCounter=0;
      kv_TapeBusy=0;
      kv_FilePointer=StartSong[i];
   }
}

void kv_CloseSampleFile(void)
{
   if(FileOpened){
      fclose(SampleFile);
      fclose(SharedSampleFile);
      FileOpened=0;
   }
}

void kv_SetNextSong(void)
{
unsigned char Temp;
   if(FileOpened){
      if(SongPositions[kv_FilePointer] & 0x80) kv_Beep=0;
      else kv_Beep=1;
      Temp=SongPositions[kv_FilePointer] & 0x7f;
      if(Temp<10) kv_SharedData=1;
      else kv_SharedData=0;
      kv_SongCounter=((unsigned long int)(SongStart[Temp+1]-SongStart[Temp]));
      if(kv_SharedData) fseek(SharedSampleFile, SongStart[Temp], SEEK_SET);
      else fseek(SampleFile, SongStart[Temp], SEEK_SET);
      kv_FilePointer++;
      kv_TapeBusy=1;
   }else{
      kv_Beep=1;
      kv_TapeBusy=1;
      kv_SongCounter=80*262;    /* delay needed for Harmony without tape */
   }
}

void kv_GetNextSampleByte(void)
{
static int oddeven = 0;
   if(kv_SongCounter==0) SampleByte = (db) 0x80;
   else{
      oddeven=oddeven^1;
      if(oddeven&1){
         kv_SongCounter--;
         if((kv_SongCounter>262*48)||(kv_Beep==0)) kv_TapeBusy=1;
         else kv_TapeBusy=0;
         if(FileOpened){
            if(kv_SharedData) SampleByte=getc(SharedSampleFile);
            else SampleByte=getc(SampleFile);
         }else SampleByte = (db) 0x80;
         if((kv_Beep==0)&&(kv_SongCounter==0)) kv_SetNextSong();
      }
   }
}

