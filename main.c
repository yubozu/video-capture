#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>
#include "picture_t.h"
#include "capture.h"

static int recording;

void stop_recording(int param)
{
	recording = 0;
}
int main()
{
	int i;
	struct picture_t pic;
	errno = 0;
	if(!camera_init(&pic))
		goto error_cam;
	if(!camera_on())
		goto error_cam_on;
	if(signal(SIGINT, stop_recording) == SIG_ERR){
		fprintf(stderr,"signal() failed\n");
		goto error_signal;
	}
	printf("Press ctrl-c to stop recording...\n");
	recording = 1;
	for(i=0; recording; i++){
		if(!camera_get_frame(&pic))
			break;
		// output pic.
	}
	printf("\n%d frames recorded\n", i);

error_signal:
	camera_off();
error_cam_on:
	camera_close();
error_cam:
	return 0;
}
