#include "krad_system.h"

int do_shutdown;
int verbose;

void failfast (char* format, ...) {

	va_list args;


	va_start(args, format);
	vfprintf(stdout, format, args);
	va_end(args);

	exit (1);
	
}

void printke (char* format, ...) {

	va_list args;

	printf ("\nERROR!\n");

	va_start(args, format);
	vfprintf(stdout, format, args);
	va_end(args);
}

void printk (char* format, ...) {

	va_list args;

	if (verbose) {
		va_start(args, format);
		vfprintf(stdout, format, args);
		va_end(args);
	}
}


void krad_system_init () {

	do_shutdown = 0;
	verbose = 1;

}

void krad_system_daemonize () {

	pid_t pid, sid;

	close (STDIN_FILENO);
	close (STDOUT_FILENO);
	close (STDERR_FILENO);

	pid = fork();

	if (pid < 0) {
		exit (EXIT_FAILURE);
	}

	if (pid > 0) {
		exit (EXIT_SUCCESS);
	}
	
	umask(0);
 
	sid = setsid();
	
	if (sid < 0) {
		exit(EXIT_FAILURE);
	}

	if ((chdir("/")) < 0) {
		exit(EXIT_FAILURE);
	}
        
}

int krad_valid_sysname (char *sysname) {
	
	int i = 0;
	char j;
	
	char requirements[512];
	
	sprintf (requirements, "sysname's must be atleast %d characters long, only lowercase letters and numbers, "
						    "begin with a letter, and no longer than %d characters.",
						    KRAD_SYSNAME_MIN, KRAD_SYSNAME_MAX);
	
	
	if (strlen(sysname) < KRAD_SYSNAME_MIN) {
		fprintf (stderr, "sysname %s is invalid, too short!\n", sysname);
		fprintf (stderr, "%s\n", requirements);
		return 0;
	}
	
	if (strlen(sysname) > KRAD_SYSNAME_MAX) {
		fprintf (stderr, "sysname %s is invalid, too long!\n", sysname);
		fprintf (stderr, "%s\n", requirements);
		return 0;
	}
	
	j = sysname[i];
	if (!((isalpha (j)) && (islower (j)))) {
		fprintf (stderr, "sysname %s is invalid, must start with a lowercase letter!\n", sysname);
		fprintf (stderr, "%s\n", requirements);
		return 0;
	}
	i++;

	while (sysname[i]) {
		j = sysname[i];
			if (!isalnum (j)) {
				fprintf (stderr, "sysname %s is invalid, alphanumeric only!\n", sysname);
				fprintf (stderr, "%s\n", requirements);
				return 0;
			}
			if (isalpha (j)) {
				if (!islower (j)) {
					fprintf (stderr, "Sysname %s is invalid lowercase letters only!\n", sysname);
					fprintf (stderr, "%s\n", requirements);
					return 0;
				}
			}
		i++;
	}

	return 1;

}