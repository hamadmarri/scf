/* public domain, do as you wish
 * http://www.tedunangst.com/flak/post/sct-set-color-temperature
 * 
 * Edited by: Hamad Al Marri
 */
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xrandr.h>

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

void
usage()
{
	printf("Usage: sct [r.r g.g b.b]\n"
		"Value must be in a range from 0.0-1.0\n"
		"If no arguments are passed sct resets the display to the default (1.0 1.0 1.0)\n"
		"If -h is passed sct will display this usage information\n");
	exit(0);
}
int
main(int argc, char **argv)
{
	Display *dpy = XOpenDisplay(NULL);
	int screen = DefaultScreen(dpy);
	Window root = RootWindow(dpy, screen);
	double gammar = 1.0;
	double gammag = 1.0;
	double gammab = 1.0;

	XRRScreenResources *res = XRRGetScreenResourcesCurrent(dpy, root);

	if (argc > 1) {
		if (strcmp(argv[1], "-h") == 0)
			usage();

		gammar = atof(argv[1]);
		gammag = atof(argv[2]);
		gammab = atof(argv[3]);

		printf("%f %f %f\n", gammar, gammag, gammab);

		if (gammar < 0.0 || gammar > 1.0)
			usage();

		if (gammag < 0.0 || gammag > 1.0)
			usage();

		if (gammab < 0.0 || gammab > 1.0)
			usage();
	}

	for (int c = 0; c < res->ncrtc; c++) {
		int crtcxid = res->crtcs[c];
		int size = XRRGetCrtcGammaSize(dpy, crtcxid);

		XRRCrtcGamma *crtc_gamma = XRRAllocGamma(size);

		for (int i = 0; i < size; i++) {
			double g = 65535.0 * i / size;
			crtc_gamma->red[i] = g * gammar;
			crtc_gamma->green[i] = g * gammag;
			crtc_gamma->blue[i] = g * gammab;
		}
		XRRSetCrtcGamma(dpy, crtcxid, crtc_gamma);

		XFree(crtc_gamma);
	}
}

