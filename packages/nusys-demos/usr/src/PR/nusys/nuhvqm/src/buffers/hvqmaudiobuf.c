/*
 * Buffer for holding the audio data
 */
#include "hvqm.h"

/*
 * Buffer for the audio data
 *     (Note) Please give it 16byte alignment.
 */
u8 hvqmAudioRecordBody[HVQM_AUDIO_DATASIZE_MAX];
