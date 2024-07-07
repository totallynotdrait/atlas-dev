#include "pc_speaker.h"

void enable_pc_speaker() {
    
    uint8_t temp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, temp | 3);  // Set first 2 bits to turn on speaker
    
}

void disable_pc_speaker() {
    uint8_t temp = inb(PC_SPEAKER_PORT);
    outb(PC_SPEAKER_PORT, temp & 0xFC);  // Clear first 2 bits to turn off speaker
}

void play_note(const NoteFrequency note, const uint32_t ms_duration) {
    PIT::set_pit_channel_mode_frequency(2, 3, note);

    // Note: Assuming PIT channel 0 is set to 1000hz or 1ms rate
    PIT::Sleep(ms_duration);
}

void rest(const uint32_t ms_duration) {
    PIT::set_pit_channel_mode_frequency(2, 3, 40);

    // Note: Assuming PIT channel 0 is set to 1000hz or 1ms rate
    PIT::Sleep(ms_duration);
}

void set_bpm(const uint32_t bpm)  {
    // Assuming PIT channel 0 is set to 1000hz or 1ms
    bpm_ms = 60000/bpm;
}

void set_time_signature(const uint8_t beats_per_measure, const beat_type_t beat_type) {
    (void)beats_per_measure;    // Silence compiler warnings

    switch(beat_type) {
        case WHOLE:
            whole_note_duration     = bpm_ms;
            half_note_duration      = bpm_ms / 2;
            quarter_note_duration   = bpm_ms / 4;
            eigth_note_duration     = bpm_ms / 8;
            sixteenth_note_duration = bpm_ms / 16;
            thirty2nd_note_duration = bpm_ms / 32;
            break;
        case HALF:
            whole_note_duration     = bpm_ms * 2;
            half_note_duration      = bpm_ms;
            quarter_note_duration   = bpm_ms / 2;
            eigth_note_duration     = bpm_ms / 4;
            sixteenth_note_duration = bpm_ms / 8;
            thirty2nd_note_duration = bpm_ms / 16;
            break;
        case QUARTER:
            whole_note_duration     = bpm_ms * 4;
            half_note_duration      = bpm_ms * 2;
            quarter_note_duration   = bpm_ms;
            eigth_note_duration     = bpm_ms / 2;
            sixteenth_note_duration = bpm_ms / 4;
            thirty2nd_note_duration = bpm_ms / 8;
            break;
        case EIGTH:
            whole_note_duration     = bpm_ms * 8;
            half_note_duration      = bpm_ms * 4;
            quarter_note_duration   = bpm_ms * 2;
            eigth_note_duration     = bpm_ms;
            sixteenth_note_duration = bpm_ms / 2;
            thirty2nd_note_duration = bpm_ms / 4;
            break;
        case SIXTEENTH:
            whole_note_duration     = bpm_ms * 16;
            half_note_duration      = bpm_ms * 8;
            quarter_note_duration   = bpm_ms * 4;
            eigth_note_duration     = bpm_ms * 2;
            sixteenth_note_duration = bpm_ms;
            thirty2nd_note_duration = bpm_ms / 2;
            break;
        case THIRTY2ND:
            whole_note_duration     = bpm_ms * 32;
            half_note_duration      = bpm_ms * 16;
            quarter_note_duration   = bpm_ms * 8;
            eigth_note_duration     = bpm_ms * 4;
            sixteenth_note_duration = bpm_ms * 2;
            thirty2nd_note_duration = bpm_ms;
            break;
    }
}

void whole_note(const NoteFrequency note)
{
    play_note(note, whole_note_duration);
}

void half_note(const NoteFrequency note)
{
    play_note(note, half_note_duration);
}

void quarter_note(const NoteFrequency note)
{
    play_note(note, quarter_note_duration);
}

void eigth_note(const NoteFrequency note)
{
    play_note(note, eigth_note_duration);
}

void sixteenth_note(const NoteFrequency note)
{
    play_note(note, sixteenth_note_duration);
}

void thirty2nd_note(const NoteFrequency note)
{
    play_note(note, thirty2nd_note_duration);
}

void whole_rest(void)
{
    rest(whole_note_duration);
}

void half_rest(void)
{
    rest(half_note_duration);
}

void quarter_rest(void)
{
    rest(quarter_note_duration);
}

void eigth_rest(void)
{
    rest(eigth_note_duration);
}

void sixteenth_rest(void)
{
    rest(sixteenth_note_duration);
}

void thirty2nd_rest(void)
{
    rest(thirty2nd_note_duration);
}

void dotted_eigth_note(const NoteFrequency note)
{
    play_note(note, eigth_note_duration + sixteenth_note_duration);
}

void eigth_triplet(const NoteFrequency note1, const NoteFrequency note2, const NoteFrequency note3)
{
    uint32_t temp = quarter_note_duration / 3;

    play_note(note1, temp);
    play_note(note2, temp);
    play_note(note3, temp);
}