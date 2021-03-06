#include <stdio.h>
#include <media/AudioTrack.h>
#include <math.h>
#include <binder/IPCThreadState.h>
#include <audio_utils/roundup.h>

void callback(int event, void* user, void *info)
{
    (void)user; (void)info;
    printf("callback: event = %d\n", event);
}

void usage(const char *cmd)
{
    printf("Usage: %s freq streamtype flag\n", cmd);
}

int main(int argc, char *argv[])
{
    int freq = 1000;
    audio_stream_type_t stream_type = AUDIO_STREAM_MUSIC;
    audio_output_flags_t flag = AUDIO_OUTPUT_FLAG_NONE;

    if (argc == 2 && (strcmp("-h", argv[1]) == 0)) {
        usage(argv[0]);
        return 0;
    }

    if (argc >= 2) {
        freq = atoi(argv[1]);
    }

    if (argc >= 3) {
        stream_type = (audio_stream_type_t)atoi(argv[2]);
    }

    if (argc >= 4) {
        flag = (audio_output_flags_t)atoi(argv[3]);
    }

    int16_t buffer[960 * 2];
    android::AudioTrack *at = new android::AudioTrack();
    android::status_t status = at->set(stream_type,
                                       48000,
                                       AUDIO_FORMAT_PCM_16_BIT,
                                       AUDIO_CHANNEL_OUT_STEREO,
                                       960,
                                       flag,
                                       callback,
                                       NULL,
                                       0,
                                       NULL,
                                       true,
                                       AUDIO_SESSION_ALLOCATE,
                                       android::AudioTrack::TRANSFER_SYNC,
                                       NULL,
                                       -1, -1,
                                       NULL
    );

    for (int i = 0; i < 960; i++) {
        buffer[2 * i + 1] = static_cast<int16_t>(sin(2 * 3.1415926 * i * freq / 48000) * 32767 * 1);
        buffer[2 * i] = static_cast<int16_t>(sin(2 * 3.1415926 * i * freq / 48000) * 32767 * 1);
    }
    printf("AudioTrack create: %d\n", status);
    status = at->start();
    printf("AudioTrack start: %d\n", status);
    at->dump(STDOUT_FILENO, android::Vector<android::String16>());
    while (1) {
        at->write(buffer, sizeof(buffer));
    }

	return 0;
}
