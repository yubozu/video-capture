int camera_init(struct picture_t *out_info);
int camera_on();
int camera_get_frame(struct picture_t *pic);
int camera_off();
void camera_close();