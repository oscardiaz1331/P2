#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "vad.h"
#include "pav_analysis.h"

const float FRAME_TIME = 10.0F; /* in ms. */

/* 
 * As the output state is only ST_VOICE, ST_SILENCE, or ST_UNDEF,
 * only this labels are needed. You need to add all labels, in case
 * you want to print the internal state in string format
 */

const char *state_str[] = {
  "MV", "MS", "S", "V", "INIT"
};

const char *state2str(VAD_STATE st) {
  return state_str[st];
}

/* Define a datatype with interesting features */
typedef struct {
  float zcr;
  float p;
  float am;
} Features;

/* 
 * TODO: Delete and use your own features!
 */

Features compute_features(const float *x, int N) {
  /*
   * Input: x[i] : i=0 .... N-1 
   * Ouput: computed features
   */
  /* 
   * DELETE and include a call to your own functions
   *
   * For the moment, compute random value between 0 and 1 
   */
  Features feat;
  //feat.zcr = compute_zcr(x,N,fm);
  feat.p = compute_power(x,N);
  feat.am = compute_am(x,N);
  return feat;
}

/* 
 * TODO: Init the values of vad_data
 */

VAD_DATA * vad_open(float rate,float alfa1,float alfa2, int dur_max, int dur_min_v,int dur_min_s) {
  VAD_DATA *vad_data = malloc(sizeof(VAD_DATA));
  vad_data->state = ST_INIT;
  vad_data->sampling_rate = rate;
  vad_data->frame_length = rate * FRAME_TIME * 1e-3;
  vad_data->alfa1 = alfa1;
  vad_data->alfa2 = alfa2;
  vad_data->s=0;
  vad_data->us=0;
  vad_data->uv=0;
  vad_data->v=0;
  vad_data->dur_max=dur_max;
  vad_data->dur_min_v=dur_min_v;
  vad_data->dur_min_s=dur_min_s;
  return vad_data;
}

VAD_STATE vad_close(VAD_DATA *vad_data) {
  /* 
   * TODO: decide what to do with the last undecided frames
   */
  //Decidimos ponerlo a silencio debido a que es lo más habitual
  VAD_STATE state = vad_data->state;
  if(state==ST_MV || state==ST_MS){ 
    state=ST_SILENCE;
  }
  free(vad_data);
  return state;
}

unsigned int vad_frame_size(VAD_DATA *vad_data) {
  return vad_data->frame_length;
}

/* 
 * TODO: Implement the Voice Activity Detection 
 * using a Finite State Automata
 */

VAD_STATE vad(VAD_DATA *vad_data, float *x) {

  /* 
   * TODO: You can change this, using your own features,
   * program finite state automaton, define conditions, etc.
   */

  Features f = compute_features(x, vad_data->frame_length);
  vad_data->last_feature = f.p; /* save feature, in case you want to show */

  switch (vad_data->state) {
  case ST_INIT:
    vad_data->state = ST_SILENCE;
    vad_data->umbral1=f.p+ vad_data->alfa1;
    vad_data->umbral2=vad_data->umbral1+vad_data->alfa2;
    break;

  case ST_SILENCE:
    if (f.p > vad_data->umbral2) vad_data->state = ST_MV;
    else vad_data->s+=1;
    break;

  case ST_VOICE:
    if (f.p < vad_data->umbral1) vad_data->state = ST_MS;
    else vad_data->v+=1;
    break;
 //Caso de MAYBE VOICE
  case ST_MV:
  //Miramos si es menor al umbral 1, osea, ruido/silencio.
    if(f.p<vad_data->umbral1 || vad_data->us>=vad_data->dur_max){
      vad_data->state=ST_SILENCE;
      vad_data->uv=0;
      vad_data->us=0;
    }
    //Miramos si supera durante un tiempo el umbral 2, sino podemos decir que es un ruido alto puntual y no voz
    if(f.p>vad_data->umbral2) {
      if(vad_data->uv>vad_data->dur_min_v){  //Esta duración debe ser corta   
        vad_data->state=ST_VOICE;
        vad_data->uv=0;
        vad_data->us=0;
      }else{
      vad_data->us+=1;
      vad_data->uv+=1;
      }
    }else{
      vad_data->uv=0;
      vad_data->us+=1;
    }
    break;
  case ST_MS:   
      //A continuación miramos si se sobrepasa la duración máxima
      if(f.p>=vad_data->umbral1){ 
        vad_data->state=ST_VOICE;
        vad_data->us=0;
      }else if(f.p<vad_data->umbral1 && vad_data->dur_min_s>vad_data->us){
          vad_data->us+=1;
      }else{
        vad_data->state=ST_SILENCE;
        vad_data->us=0;
      }
    break;
  }
  if (vad_data->state == ST_SILENCE ||
      vad_data->state == ST_VOICE||
      vad_data->state == ST_MS ||
      vad_data->state == ST_MV) return vad_data->state;
  else return ST_MS;
}

void vad_show_state(const VAD_DATA *vad_data, FILE *out) {
  fprintf(out, "%d\t%f\n", vad_data->state, vad_data->last_feature);
}
