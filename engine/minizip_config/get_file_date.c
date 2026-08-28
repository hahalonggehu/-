#include <sys/stat.h>
#include <time.h>

#if defined(_WIN32)
#  define JY_LOCALTIME_R(t, tt) (localtime_s((tt), (t)) == 0)
#else
#  define JY_LOCALTIME_R(t, tt) (localtime_r((t), (tt)) != NULL)
#endif

/* Compute DOS date/time from tm; used by JY mod scripts via get_file_date(). */
static void dos_from_tm(const struct tm* t, unsigned long* d, unsigned long* ti)
{
    if (t == NULL)
    {
        *d = 0;
        *ti = 0;
        return;
    }
    int year = t->tm_year - 80; /* years since 1980 */
    if (year < 0) { year = 0; }
    if (year > 127) { year = 127; }
    *d = ((unsigned long)(t->tm_mday & 0x1f))
         | ((unsigned long)((t->tm_mon + 1) & 0x0f) << 5)
         | ((unsigned long)(year & 0x7f) << 9);
    *ti = ((unsigned long)((t->tm_sec / 2) & 0x1f))
          | ((unsigned long)(t->tm_min & 0x3f) << 5)
          | ((unsigned long)(t->tm_hour & 0x1f) << 11);
}

void get_file_date(const char* filename, unsigned long* dos_date)
{
    struct stat st;
    struct tm t;
    unsigned long dos_time;
    if (dos_date == NULL)
    {
        return;
    }
    if (stat(filename, &st) != 0 || !JY_LOCALTIME_R(&st.st_mtime, &t))
    {
        *dos_date = 0;
        return;
    }
    dos_from_tm(&t, dos_date, &dos_time);
}