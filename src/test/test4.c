#include <dnswire/writer.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "create_dnstap.c"

int main(int argc, const char* argv[])
{
    if (argc < 2) {
        return 1;
    }

    FILE* fp = fopen(argv[1], "w");
    if (!fp) {
        return 2;
    }

    struct dnswire_writer writer = DNSWIRE_WRITER_INITIALIZER;

    struct dnstap d = DNSTAP_INITIALIZER;
    create_dnstap(&d, "test4");

    dnswire_writer_set_dnstap(writer, &d);

    while (1) {
        enum dnswire_result res = dnswire_writer_fwrite(&writer, fp);
        switch (res) {
        case dnswire_ok:
            break;

        case dnswire_again:
        case dnswire_need_more:
            continue;

        default:
            fprintf(stderr, "dnswire_reader_add() error\n");
            return 1;
        }
        break;
    }

    create_dnstap(&d, "test4");

    while (1) {
        enum dnswire_result res = dnswire_writer_fwrite(&writer, fp);
        switch (res) {
        case dnswire_ok:
            break;

        case dnswire_again:
        case dnswire_need_more:
            continue;

        default:
            fprintf(stderr, "dnswire_reader_add() error\n");
            return 1;
        }
        break;
    }

    if (dnswire_writer_stop(&writer) != dnswire_ok) {
        fprintf(stderr, "dnswire_writer_stop() failed\n");
        return 1;
    }

    while (1) {
        enum dnswire_result res = dnswire_writer_fwrite(&writer, fp);
        switch (res) {
        case dnswire_ok:
        case dnswire_endofdata:
            break;

        case dnswire_again:
        case dnswire_need_more:
            continue;

        default:
            fprintf(stderr, "dnswire_reader_add() error\n");
            return 1;
        }
        break;
    }

    fclose(fp);
    return 0;
}
