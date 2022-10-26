#include "flpan~.h"

void ext_main(void *r)
{
	fl_pan_class = class_new("flpan~", (method)fl_pan_new, (method)fl_pan_free, sizeof(t_fl_pan), 0, A_GIMME, 0);

	class_addmethod(fl_pan_class, (method)fl_pan_dsp64, "dsp64", A_CANT, 0);
	class_addmethod(fl_pan_class, (method)fl_pan_float, "float", A_FLOAT, 0);
	class_addmethod(fl_pan_class, (method)fl_pan_int, "int", A_LONG, 0);
	class_addmethod(fl_pan_class, (method)fl_pan_assist, "assist", A_CANT, 0);
	class_addmethod(fl_pan_class, (method)fl_pan_bang, "bang", 0);

	class_dspinit(fl_pan_class);

	class_register(CLASS_BOX, fl_pan_class);
}

void *fl_pan_new(t_symbol *s, short argc, t_atom *argv)
{
	t_fl_pan *x = (t_fl_pan *)object_alloc(fl_pan_class);

	dsp_setup((t_pxobject *)x, 2);
	x->m_outlet2 = outlet_new((t_object *)x, "signal"); //right
	x->m_outlet1 = outlet_new((t_object *)x, "signal"); //left
	x->obj.z_misc |= Z_NO_INPLACE;

	x->fs = sys_getsr();

	return x;
}

void fl_pan_assist(t_fl_pan *x, void *b, long msg, long arg, char *dst)
{
	if (msg == ASSIST_INLET) {
		switch (arg) {
		case I_AUDIOIN: sprintf(dst, "(sig~) audio in"); break;
		case I_PAN: sprintf(dst, "(sig~) pan"); break;
		}
	}
	else if (msg == ASSIST_OUTLET) {
		switch (arg) {
		case O_AUDIOOUTL: sprintf(dst, "(sig~) audio out L"); break;
		case O_AUDIOOUTR: sprintf(dst, "(sig~) audio out R"); break;
		}
	}
}

void fl_pan_float(t_fl_pan *x, double f){}

void fl_pan_bang(t_fl_pan *x){}

void fl_pan_int(t_fl_pan *x, long n){}

void fl_pan_free(t_fl_pan *x)
{
	dsp_free((t_pxobject *)x);
}

void fl_pan_dsp64(t_fl_pan *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	x->audioin_connected = count[0];
	x->panin_connected = count[1];

	if (x->fs != samplerate) { x->fs = samplerate; }
	
	object_method(dsp64, gensym("dsp_add64"), x, fl_pan_perform64, 0, NULL);
}

void fl_pan_perform64(t_fl_pan *x, t_object *dsp64, double **inputs, long numinputs, double **outputs, long numoutputs,
	long vectorsize, long flags, void *userparams)
{
	t_double *audioin = inputs[0];
	t_double *panin = inputs[1];
	t_double *outleft = outputs[0];
	t_double *outright = outputs[1];

	long n = vectorsize;
	double fs = x->fs;

	double pan = 0.;
	double samp = 0.;
	double normpan = 0.;

	while (n--){

		if (x->audioin_connected) { samp = *audioin++; }
		if (x->panin_connected) { pan = *panin++; }

		normpan = MIN(1., MAX(-1., pan));

		*outleft++ = samp * cos((1 + normpan) * PIOVER4);
		*outright++ = samp * cos((1 - normpan) * PIOVER4);

	}
}