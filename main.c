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
#include "simplerecorder.h"

static int recording;

void stop_recording(int param)
{
	recording = 0;
}
int main()
{
	int i;
	struct picture_t pic;
	struct encoded_pic_t encoded_pic;
	errno = 0;
	if(!camera_init(&pic))
		goto error_cam;
	if(!encoder_init(&pic)){
		fprintf(stderr,"failed to initialize encoder\n");
		goto error_encoder;
	}
	if(!output_init(&pic))
		goto error_output;
	if(!encoder_encode_headers(&encoded_pic))
		goto error_output;
	if(!output_write_headers(&encoded_pic))
		goto error_output;
	encoder_release(&encoded_pic);
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
		if(!encoder_encode_frame(&pic, &encoded_pic))
			break;
		if(!output_write_frame(&encoded_pic))
			break;
		encoder_release(&encoded_pic);
	}
	printf("\n%d frames recorded\n", i);

error_signal:
	camera_off();
error_cam_on:
	output_close();
error_output:
	encoder_close();
error_encoder:
	camera_close();
error_cam:
	return 0;
}
