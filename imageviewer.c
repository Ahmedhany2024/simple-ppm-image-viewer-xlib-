#include <X11/X.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <stdlib.h>
 

Display *display;
Window win;
Visual *vis;
XImage *img;
int main(int argc, char **argv)
{  
  FILE *pf;
  pf = fopen(argv[1], "r");
  int x = 0, y = 0, maxval = 0;
  char magic[3];

  fscanf(pf, "%2s", magic);          // "P6"

  int c;
  while ((c = fgetc(pf)) != EOF) {
    if (c == '#') { while ((c = fgetc(pf)) != '\n' && c != EOF); }
    else if (c > ' ') { ungetc(c, pf); break; }
  }

  fscanf(pf, "%d %d %d", &x, &y, &maxval);
  fgetc(pf);                         

  int total = x * y * 3;
  unsigned char *rgb = malloc(total);
  fread(rgb, 1, total, pf);
  fclose(pf);

  // display and window creation //
  display = XOpenDisplay(NULL);
  
  int screen = DefaultScreen(display);
  
  Window root = RootWindow(display, screen);
  
  GC gc = DefaultGC(display, screen);
  
  int depth = DefaultDepth(display, screen);
  
  vis = DefaultVisual(display, screen);
  
  char *data = malloc(x * y * 4);

  // window and image sizes //

  int scr_x = DisplayWidth(display, screen);
  int scr_y = DisplayHeight(display, screen);

  
  
  img = XCreateImage(display, vis, depth , ZPixmap, 0, data, x, y, 32, 0);

  int img_x = (scr_x - img->width) / 2;
  int img_y = (scr_y - img->height) /2;
  
  win = XCreateSimpleWindow(display, root, 0, 0, scr_x, scr_y, 15, BlackPixel(display, screen), WhitePixel(display, screen));
  
  XStoreName(display, win, "hash image viewer");
  
  // loop printing the rgb index//


  for(int h =0 ; h < x ; h++){
    for (int k = 0; k < y; k++) {
     int idx = (h * x + k) * 3;
        unsigned int r = rgb[idx + 0];
        unsigned int g = rgb[idx + 1];
        unsigned int b = rgb[idx + 2];

        unsigned long pixel = (r << 16) | (g << 8) | b;  // 0xRRGGBB
        XPutPixel(img, k, h, pixel);
        
    }
    
  }
  

 
  XSelectInput(display, win, ExposureMask | KeyPressMask);
  
  XMapWindow(display, win);

  XEvent ev;
  while (1) {
    XNextEvent(display, &ev);
    if (ev.type == Expose)
      XPutImage(display, win, gc, img, 0, 0, img_x, img_y, img->width, img->height);
    if (ev.type == KeyPress) break;
  }

  free(rgb);

  
  XUnmapWindow(display, win);

  XDestroyWindow(display, win);

  XCloseDisplay(display);
  return 0;


}