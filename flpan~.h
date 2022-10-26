#ifndef fl_pan_h
#define fl_pan_h

#include "ext.h"
#include "z_dsp.h"
#include "ext_obex.h"
#include <math.h>

#define PIOVER4 0.78539816339744830961

typedef struct _fl_pan {
	t_pxobject obj;

	short audioin_connected;
	short panin_connected;

	double fs;

	void *m_outlet1;
	void *m_outlet2;

} t_fl_pan;

enum INLETS { I_AUDIOIN, I_PAN, NUM_INLETS };
enum OUTLETS { O_AUDIOOUTL, O_AUDIOOUTR, NUM_OUTLETS };

static t_class *fl_pan_class;

void *fl_pan_new(t_symbol *s, short argc, t_atom *argv);
void fl_pan_float(t_fl_pan *x, double f);
void fl_pan_bang(t_fl_pan *x);
void fl_pan_int(t_fl_pan *x, long n);
void fl_pan_assist(t_fl_pan *x, void *b, long msg, long arg, char *dst);

void fl_pan_free(t_fl_pan *x);

void fl_pan_dsp64(t_fl_pan *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void fl_pan_perform64(t_fl_pan *x, t_object *dsp64, double **inputs, long numinputs, double **outputs, long numoutputs, long vectorsize, long flags, void *userparams);

#endif
