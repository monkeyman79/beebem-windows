/****************************************************************
BeebEm - BBC Micro and Master 128 Emulator
Copyright (C) 1994  David Alan Gilbert
Copyright (C) 1997  Mike Wyatt
Copyright (C) 2001  Richard Gellman
Copyright (C) 2008  Rich Talbot-Watkins

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public 
License along with this program; if not, write to the Free 
Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA  02110-1301, USA.
****************************************************************/

/* Sound emulation for the beeb - David Alan Gilbert 26/11/94 */

#ifndef SOUND_HEADER
#define SOUND_HEADER

#ifdef WIN32
/* Always compile sound code - it is switched on and off using SoundEnabled */
#define SOUNDSUPPORT
#include <windows.h>
#endif

#define MUTED 0
#define UNMUTED 1

#include <stdio.h>

namespace SoundConfig
{
	extern enum Option
	{
		XAudio2,
		DirectSound
	}
	Selection;
}

extern int SoundDefault; // Default sound state (enabled/disabled via sound menu)
extern int SoundEnabled;    /* Sound on/off flag - will be off if DirectSound init fails */
extern int RelaySoundEnabled; // Relay Click noise enable
extern int DiscDriveSoundEnabled; // Disc drive sound enable
extern int SoundSampleRate; /* Sample rate, 11025, 22050 or 44100 Hz */
extern int SoundVolume;     /* Volume, 1(full),2,3 or 4(low) */
extern char SoundExponentialVolume;

extern int SoundTrigger; /* Cycle based trigger on sound */
extern double SoundTuning;
extern __int64 SoundCycles;

void SoundInit();
void SoundReset();

/* Called in sysvia.cc when a write to one of the 76489's registers occurs */
void Sound_RegWrite(int Value);
void DumpSound(void);
void ClickRelay(unsigned char RState);

#define SAMPLE_RELAY_ON         0
#define SAMPLE_RELAY_OFF        1
#define SAMPLE_DRIVE_MOTOR      2
#define SAMPLE_HEAD_LOAD        3
#define SAMPLE_HEAD_UNLOAD      4
#define SAMPLE_HEAD_SEEK        5
#define SAMPLE_HEAD_STEP        6
#define SAMPLE_HEAD_SEEK_CYCLES_PER_TRACK  48333  // 0.02415s per track in the sound file
#define SAMPLE_HEAD_STEP_CYCLES           100000  // 0.05s sound file
#define SAMPLE_HEAD_LOAD_CYCLES           400000  // 0.2s sound file
void PlaySoundSample(int sample, bool repeat);
void StopSoundSample(int sample);

void Sound_Trigger(int NCycles);

extern volatile BOOL bDoSound;
extern void AdjustSoundCycles(void);

void SetSound(char State);

struct AudioType {
	char Signal; // Signal type: data, gap, or tone.
	char BytePos; // Position in data byte
	bool Enabled; // Enable state of audio deooder
	int Data; // The actual data itself
	int Samples; // Samples counted in current pattern till changepoint
	char CurrentBit; // Current bit in data being processed
	char ByteCount; // Byte repeat counter
};

extern struct AudioType TapeAudio;
extern bool TapeSoundEnabled;
extern int SoundChipEnabled;
void SoundChipReset(void);
void SwitchOnSound(void);
extern int UsePrimaryBuffer;
void LoadSoundUEF(FILE *SUEF);
void SaveSoundUEF(FILE *SUEF);
extern int PartSamples;
extern int SBSize;
void MuteSound(void);
#endif
