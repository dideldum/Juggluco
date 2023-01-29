/*      This file is part of Juggluco, an Android app to receive and display         */
/*      glucose values from Freestyle Libre 2 and 3 sensors.                         */
/*                                                                                   */
/*      Copyright (C) 2021 Jaap Korthals Altes <jaapkorthalsaltes@gmail.com>         */
/*                                                                                   */
/*      Juggluco is free software: you can redistribute it and/or modify             */
/*      it under the terms of the GNU General Public License as published            */
/*      by the Free Software Foundation, either version 3 of the License, or         */
/*      (at your option) any later version.                                          */
/*                                                                                   */
/*      Juggluco is distributed in the hope that it will be useful, but              */
/*      WITHOUT ANY WARRANTY; without even the implied warranty of                   */
/*      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                         */
/*      See the GNU General Public License for more details.                         */
/*                                                                                   */
/*      You should have received a copy of the GNU General Public License            */
/*      along with Juggluco. If not, see <https://www.gnu.org/licenses/>.            */
/*                                                                                   */
/*      Fri Jan 27 12:36:58 CET 2023                                                 */


#include <vector>
#include <string_view>
#include <cstring>
#include <cinttypes>
#include <cmath>
#include "settings.h"
#include <jni.h>
//#include "fromjava.h"
#include "config.h"
#include "datbackup.h"
extern Backup *backup;
#define fromjava(x) Java_tk_glucodata_Natives_ ##x
extern "C" JNIEXPORT jfloat  JNICALL   fromjava(getweight)(JNIEnv *env, jclass cl,jint pos) {
	return settings->getlabelweight(pos)  ;
	}
extern "C" JNIEXPORT jfloat  JNICALL   fromjava(getprec)(JNIEnv *env, jclass cl,jint pos) {
	return settings->getlabelprec(pos)  ;
	}

extern "C" JNIEXPORT jint  JNICALL   fromjava(getunit)(JNIEnv *env, jclass cl) {
	return settings->data()->unit;
	}
extern "C" JNIEXPORT jfloat  JNICALL   fromjava(graphlow)(JNIEnv *env, jclass cl) {
	return gconvert(settings->data()->glow);
	}
extern "C" JNIEXPORT jfloat  JNICALL   fromjava(graphhigh)(JNIEnv *env, jclass cl) {
	return gconvert(settings->data()->ghigh);
	}
extern "C" JNIEXPORT jfloat  JNICALL   fromjava(targetlow)(JNIEnv *env, jclass cl) {
	return gconvert(settings->data()->tlow);
	}
extern "C" JNIEXPORT jfloat  JNICALL   fromjava(targethigh)(JNIEnv *env, jclass cl) {
	return gconvert(settings->data()->thigh);
	}
extern "C" JNIEXPORT jfloat  JNICALL   fromjava(alarmlow)(JNIEnv *env, jclass cl) {
	return gconvert(settings->data()->alow);
	}
extern "C" JNIEXPORT jfloat  JNICALL   fromjava(alarmhigh)(JNIEnv *env, jclass cl) {
	return gconvert(settings->data()->ahigh);
	}
extern "C" JNIEXPORT jboolean  JNICALL   fromjava(hasalarmhigh)(JNIEnv *env, jclass cl) {
	return settings->data()->highalarm;
	}
extern "C" JNIEXPORT jboolean  JNICALL   fromjava(hasalarmlow)(JNIEnv *env, jclass cl) {
	return settings->data()->lowalarm;
	}
extern "C" JNIEXPORT jboolean  JNICALL   fromjava(hasalarmloss)(JNIEnv *env, jclass cl) {
	return settings->data()->lossalarm;
	}
extern "C" JNIEXPORT jboolean  JNICALL   fromjava(hasvaluealarm)(JNIEnv *env, jclass cl) {
	return settings->data()->availablealarm;
	}
extern "C" JNIEXPORT void  JNICALL   fromjava(setunit)(JNIEnv *env, jclass cl,jint unit) {
	settings->setunit(unit);
	}
extern "C" JNIEXPORT jboolean  JNICALL   fromjava(staticnum)(JNIEnv *env, jclass cl) {
	 return settings->staticnum(); 
	}

bool Settings::staticnum() const {
	 return data()->nochangenum;
	}
extern "C" JNIEXPORT void  JNICALL   fromjava(setstaticnum)(JNIEnv *env, jclass cl,jboolean val) {
	settings->data()->nochangenum=val;
	}

	/*
extern bool haslib;
bool haslib=false;
extern "C" JNIEXPORT void  JNICALL   fromjava(sethaslib)(JNIEnv *env, jclass cl,jboolean val) {
	haslib=val;
	}
*/
extern "C" JNIEXPORT void  JNICALL   fromjava(setalarms)(JNIEnv *env, jclass cl,jfloat alow, jfloat ahigh, jboolean lowalarm, jboolean highalarm, jboolean availablealarm,jboolean lossalarm) {
settings->setalarms(roundf(settings->tomgperL(alow)), roundf(settings->tomgperL(ahigh)),lowalarm, highalarm, availablealarm,lossalarm) ;
	}
/*
bool hasnotiset() {
	if(!settings)
		return false;
	const auto set=settings->data();
	return (!set->dontshowalways)||
		set->lowalarm||
		set->highalarm||
		set->lossalarm||
		set->availablealarm;
	}
*/

extern "C" JNIEXPORT void  JNICALL   fromjava(setsettings)(JNIEnv *env, jclass cl,jint unit,jfloat glow, jfloat ghigh, jfloat tlow, jfloat thigh) {
	settings->setunit(unit);
	settings->setranges( roundf(settings->tomgperL(glow)), roundf(settings->tomgperL(ghigh)), roundf(settings->tomgperL(tlow)), roundf(settings->tomgperL(thigh))) ;
	}
/*	
extern "C" JNIEXPORT jboolean  JNICALL   fromjava(toCalendarApp)(JNIEnv *env, jclass cl) {
	return settings->data()->callapp;
	}
	*/

extern void mkheights();
extern "C" JNIEXPORT jboolean  JNICALL   fromjava(setlabel)(JNIEnv *env, jclass cl,jint index,jstring jlabel, jfloat prec,jfloat weight) {
	jint len = env->GetStringUTFLength( jlabel);
	if(len>11)
		return false;

	jint jlen = env->GetStringLength( jlabel);
	env->GetStringUTFRegion(jlabel, 0,jlen, settings->data()->vars[index].name);
	settings->data()->vars[index].name[len]='\0';
	settings->data()->vars[index].prec=prec;
	settings->data()->vars[index].weight=settings->tomgperL(weight);
	if(index>=settings->varcount())  {
		settings->varcount()=index+1;
		mkheights() ;
		}

	settings->data()->sendlabels=true;
	settings->updated();
	return true;
	}

extern "C" JNIEXPORT void  JNICALL   fromjava(havesendlabels)(JNIEnv *env, jclass cl) {
	settings->data()->sendlabels=false;
	}
extern "C" JNIEXPORT void  JNICALL   fromjava(receivedcuts)(JNIEnv *env, jclass cl) {
	settings-> data()->sendcuts=false;
	}
extern "C" JNIEXPORT jboolean  JNICALL   fromjava(sendcuts)(JNIEnv *env, jclass cl) {
	return settings-> data()->sendcuts;
	}
extern "C" JNIEXPORT void  JNICALL   fromjava(setnrlabel)(JNIEnv *env, jclass cl,jint index) {
	if(index>=0) {
		settings->varcount()=index;
		settings->data()->sendlabels=true;
		mkheights();
		settings->updated();

		}
	}
extern "C" JNIEXPORT jboolean  JNICALL   fromjava(shouldsendlabels)(JNIEnv *env, jclass cl) {
		return settings->data()->sendlabels;
	}

/*
static constexpr const char starthtml[]= R"(<h1>Modified device</h1>
<p>One of the libraries used by this application has a BUG that makes
it crash if it detects certain files. Your device contains some of
these files. This program contains a hack to circumvent this BUG, but
it is probably better to make these files undetectable in some other
way. Magisk, for example, has the option to hide root for certain
applications (Magiskhide or Denylist) and change its own name, both 
are needed. In your case it has problems with the following file)";
*/
#ifndef WEAROS
static constexpr const char endstart[]=R"(:</p>)";

extern bool wrongfiles() ;
extern		std::vector<std::string_view> usedfiles;
#include "curve/jugglucotext.h"
extern bool rootcheck;
extern "C" JNIEXPORT jstring  JNICALL   fromjava(advanced)(JNIEnv *env, jclass cl) {
	if(!rootcheck)
		return nullptr;
	if(wrongfiles()) {
		const int textlen= usedtext->advancedstart.size();
		int totlen=textlen+sizeof(endstart)-1+(usedfiles.size()>1?1:0);
		const char tus[]="<br/>\n";
		constexpr int tuslen=sizeof(tus)-1;
		for(auto el:usedfiles)
			totlen+=el.size()+tuslen;
		char buf[totlen];
		int it=textlen;
		memcpy(buf,usedtext->advancedstart.data(),it);
		if(usedfiles.size()>1&&usedtext->add_s)
			buf[it++]='s';
		memcpy(buf+it,endstart,sizeof(endstart)-1);
		it+= sizeof(endstart)-1;
		for(auto el:usedfiles) {
			memcpy(buf+it,el.data(),el.size());
			it+=el.size();
			memcpy(buf+it,tus,tuslen);
			it+=tuslen;
			}
		buf[totlen-1]='\0';
		return env->NewStringUTF(buf);
		}
	else
		return nullptr;
	}
#endif
/*
extern "C" JNIEXPORT void  JNICALL   fromjava(setlibversion)(JNIEnv *env, jclass cl,jstring jversion) {
	jint len = env->GetStringUTFLength( jversion);
	env->GetStringUTFRegion(jversion, 0,len, settings->data()->libversion);
	settings->data()->libversion[len]='\0';
	}
	*/
extern "C" JNIEXPORT jfloat  JNICALL   fromjava(getPrecision)(JNIEnv *env, jclass cl,jint index) {
	return settings->data()->vars[index].prec;
	}

extern "C" JNIEXPORT jobject  JNICALL   fromjava(getLabels)(JNIEnv *env, jclass cl) {
	int len=settings->getlabelcount();
	jclass carlist=env->FindClass("java/util/ArrayList");
	jmethodID init=env->GetMethodID(carlist,"<init>","(I)V");
	jobject arlist=env->NewObject(carlist,init,len+1);

/*	jmethodID cap=env->GetMethodID(carlist,"ensureCapacity","(I)V");
	env->CallVoidMethod(arlist,cap,len+1);*/
	jmethodID add=env->GetMethodID(carlist,"add","(Ljava/lang/Object;)Z");
	 env->DeleteLocalRef(carlist);

	 for(int i=0;i<len;i++) {
	 	    env->CallBooleanMethod(arlist,add, env->NewStringUTF(settings->getlabel(i).data()));
		  }
        env->CallBooleanMethod(arlist,add, env->NewStringUTF(""));
	return arlist;
	}
extern "C" JNIEXPORT jobject  JNICALL   fromjava(getShortcuts)(JNIEnv *env, jclass cl) {
	int len=settings->getshortcutcount();
	LOGGER("getshortcutcount=%d\n",len);
	jclass carlist=env->FindClass("java/util/ArrayList");
	jmethodID init=env->GetMethodID(carlist,"<init>","(I)V");
	jobject arlist=env->NewObject(carlist,init,len);
//	jmethodID cap=env->GetMethodID(carlist,"ensureCapacity","(I)V"); env->CallVoidMethod(arlist,cap,len);
	jmethodID add=env->GetMethodID(carlist,"add","(Ljava/lang/Object;)Z");

	 for(int i=0;i<len;i++) {
		jobject arel=env->NewObject(carlist,init,2);
		env->CallBooleanMethod(arel,add, env->NewStringUTF(settings->data()->shorts[i].name));
		env->CallBooleanMethod(arel,add, env->NewStringUTF(settings->data()->shorts[i].value));
	 	env->CallBooleanMethod(arlist,add, arel);
	 	env->DeleteLocalRef(arel);
		 }
	return arlist;
	}
extern "C" JNIEXPORT int  JNICALL   fromjava(setShortcut)(JNIEnv *env, jclass cl,jint index,jstring jname,jstring jvalue) {
	if( index>=settings->data()->shorts.size()) {
		return -5;
		}
	jint len = env->GetStringUTFLength( jname);
	if(len>11) {
		return 0;
		}

	jint jlen = env->GetStringLength( jname);
	env->GetStringUTFRegion(jname, 0,jlen, settings->data()->shorts[index].name);
	settings->data()->shorts[index].name[len]='\0';

	len = env->GetStringUTFLength( jvalue);
	if(len>11) {
		return 1;
		}
	jlen = env->GetStringLength( jvalue);
	env->GetStringUTFRegion(jvalue, 0,len, settings->data()->shorts[index].value);
	settings->data()->shorts[index].value[len]='\0';
	return -1;
	}

extern "C" JNIEXPORT void  JNICALL   fromjava(setnrshortcuts)(JNIEnv *env, jclass cl,jint nr) {
	settings->setshortcutcount(nr);
	settings->updated();
	}


extern "C" JNIEXPORT void  JNICALL   fromjava(setnodebug)(JNIEnv *env, jclass cl,jboolean val) {
	settings->setnodebug(val);
	settings->data()->nobluetooth=false;
	}

extern "C" JNIEXPORT void  JNICALL   fromjava(sethaslibrary)(JNIEnv *env, jclass cl,jboolean val) {
	settings->data()->havelibrary=val;
	}
extern "C" JNIEXPORT jboolean  JNICALL   fromjava(gethaslibrary)(JNIEnv *env, jclass cl) {
	return settings->data()->havelibrary;
	}


extern "C" JNIEXPORT jint  JNICALL   fromjava(getScreenOrientation)(JNIEnv *env, jclass cl) {
	return settings->data()->orientation;
	}

extern "C" JNIEXPORT void  JNICALL   fromjava(setScreenOrientation)(JNIEnv *env, jclass cl,jint val) {
	settings->data()->orientation=val;
	}
extern "C" JNIEXPORT jboolean  JNICALL   fromjava(getInvertColors)(JNIEnv *env, jclass cl) {
	return settings->data()->invertcolors;
	}

extern int startincolors;

extern void setinvertcolors(bool val) ;
void setinvertcolors(bool val) {
	settings->data()->invertcolors=val;
	 startincolors=val*startbackground;
	 }

extern "C" JNIEXPORT void  JNICALL   fromjava(setInvertColors)(JNIEnv *env, jclass cl,jboolean val) {
	setinvertcolors(val);
	 LOGGER("startincolors=%d\n",startincolors);
//	doinvertcolors(val);
	}

extern "C" JNIEXPORT jboolean  JNICALL   fromjava(getshowalways)(JNIEnv *env, jclass cl) {
	return !settings->data()->dontshowalways;
	}
extern "C" JNIEXPORT void  JNICALL   fromjava(setshowalways)(JNIEnv *env, jclass cl,jboolean val) {
	settings->data()->dontshowalways=!val;
	}

static const uint32_t starttime=time(nullptr);
extern "C" JNIEXPORT jboolean  JNICALL   fromjava(getaskedNotify)(JNIEnv *env, jclass cl) {
/*
	constexpr const uint32_t waittime=
#ifdef NDEBUG
	60*60
#else
	60*5
#endif
	;
	if((time(nullptr)-starttime)<waittime)
		return true; */
	return settings->data()->askedNotify;
	}
extern "C" JNIEXPORT void  JNICALL   fromjava(setaskedNotify)(JNIEnv *env, jclass cl,jboolean val) {
	settings->data()->askedNotify=val;
	}

	
extern "C" JNIEXPORT jboolean  JNICALL   fromjava(gethasgarmin)(JNIEnv *env, jclass cl) {
	return settings->data()->hasgarmin;
	}
extern "C" JNIEXPORT void  JNICALL   fromjava(sethasgarmin)(JNIEnv *env, jclass cl,jboolean val) {
	settings->data()->hasgarmin=val;
	}
extern "C" JNIEXPORT void  JNICALL   fromjava(setusebluetooth)(JNIEnv *env, jclass cl,jboolean val) {
//	settings->sensorerror=false;
	settings->data()->nobluetooth=!val;
	}
extern "C" JNIEXPORT jboolean  JNICALL   fromjava(getusebluetooth)(JNIEnv *env, jclass cl) {
	if(!settings||!settings->data())
		return true;
	return !settings->data()->nobluetooth;
	}	
extern "C" JNIEXPORT jboolean  JNICALL   fromjava(streamingAllowed)(JNIEnv *env, jclass cl) {
	if(fromjava(getusebluetooth)(env,cl))
		return true;
	return backup->sendScans(); 
	}	
extern "C" JNIEXPORT jboolean  JNICALL   fromjava(shouldwakesender)(JNIEnv *env, jclass cl) {
	if(!settings||!settings->data()||!settings->data()->nobluetooth||!backup)
		return false;
	return backup->shouldaskfordata;
	}

extern "C" JNIEXPORT jboolean  JNICALL   fromjava(getuseflash)(JNIEnv *env, jclass cl) {
	return settings->data()->flash;
	}	
extern "C" JNIEXPORT void  JNICALL   fromjava(setuseflash)(JNIEnv *env, jclass cl,jboolean val) {
	settings->data()->flash=val;
	}
extern "C" JNIEXPORT jintArray  JNICALL   fromjava(numAlarmEvents)(JNIEnv *env, jclass cl) {
	vector<int> failures=settings->numAlarmEvents();
	const int failnr= failures.size();
	if(!failnr)
		return nullptr;
	jintArray  uit=env->NewIntArray(failnr) ;
	env->SetIntArrayRegion(uit, 0,failnr, failures.data());
	return uit;
	}

extern "C" JNIEXPORT jlong  JNICALL   fromjava(firstAlarm)(JNIEnv *env, jclass cl) {
	return settings->firstAlarm()*1000LL;
	}
extern "C" JNIEXPORT void  JNICALL   fromjava(setNumAlarm)(JNIEnv *env, jclass cl,jint type,jfloat value,jint start,jint alarm) {
	settings->setnumalarm(type,value,start,alarm);
	}
extern "C" JNIEXPORT void  JNICALL   fromjava(delNumAlarm)(JNIEnv *env, jclass cl,jint pos) {
	settings->delnumalarm(pos) ;
	}
//setNumAlarm(
extern "C" JNIEXPORT  jobjectArray JNICALL   fromjava(getNumAlarm)(JNIEnv *env, jclass cl,jint pos) {
	if(pos>=settings->data()->alarmnr)
		return nullptr;
	amountalarm &alarm=settings->data()->numalarm[pos];
	jclass objectclass = env->FindClass( "java/lang/Object");
	jobjectArray jall = env->NewObjectArray( 2, objectclass, NULL);
	jclass classFloat = env->FindClass("java/lang/Float");
	jmethodID floatInit = env->GetMethodID(classFloat, "<init>", "(F)V");
	jobject jvalue = env->NewObject(classFloat, floatInit,alarm.value);
	env->SetObjectArrayElement(jall, 0, jvalue);
	jshortArray  uit=env->NewShortArray(4) ;
	env->SetShortArrayRegion(uit, 0,4,  reinterpret_cast<int16_t *>(&alarm.start));
	env->SetObjectArrayElement(jall, 1,uit);
	return jall;
	}
extern "C" JNIEXPORT jint  JNICALL   fromjava(getNumAlarmCount)(JNIEnv *env, jclass cl) {
	return settings->data()->alarmnr;
	}

extern "C" JNIEXPORT jint  JNICALL   fromjava(camerakey)(JNIEnv *env, jclass cl) {
	return settings->data()->camerakey;
	}
extern "C" JNIEXPORT void  JNICALL   fromjava(setcamerakey)(JNIEnv *env, jclass cl,jint st) {
	settings->data()->camerakey=st;
	}
	/*
#define mkrings(name)\
extern "C" JNIEXPORT jstring  JNICALL   fromjava(read##name##ring)(JNIEnv *env, jclass cl) {\
	return env->NewStringUTF(settings->data()->name##ring);\
	}\
extern "C" JNIEXPORT void  JNICALL   fromjava(write##name##ring)(JNIEnv *env, jclass cl,jstring juristr) {\
	jint len = env->GetStringUTFLength(juristr );\
	if(len>=maxuri)\
		len=maxuri-1;\
	env->GetStringUTFRegion(juristr, 0,len, settings->data()->name##ring);\
	settings->data()->name##ring[len]='\0';\
	}

mkrings(low)
mkrings(high)
mkrings(again)
mkrings(num)
*/

extern "C" JNIEXPORT jstring  JNICALL   fromjava(readring)(JNIEnv *env, jclass cl,jint type) {
	return env->NewStringUTF(settings->data()->alarms[type].uri);
	}

static void	updateflash() {
	bool doesflash=false;
	const struct ring *rings=settings->data()->alarms;
	for(int i=0;i< maxalarms;i++) {
		if(rings[i].flash)
			doesflash=true;
		}
	settings->data()->flash=doesflash;
	}
extern "C" JNIEXPORT jboolean  JNICALL   fromjava(writering)(JNIEnv *env, jclass cl,jint type,jstring juristr,jboolean sound,jboolean flash,jboolean vibration) {
    if(sound) {
	if(juristr) {
		jint len= env->GetStringUTFLength(juristr );
		if(len>=maxuri) {
			return false;
			}

		jint jlen = env->GetStringLength( juristr);
		env->GetStringUTFRegion(juristr, 0,jlen, settings->data()->alarms[type].uri);
		settings->data()->alarms[type].uri[len]='\0';
		}
	else  {
		settings->data()->alarms[type].uri[0]='\0';
		}
	}
     settings->data()->alarms[type].nosound=!sound;
     settings->data()->alarms[type].novibration=!vibration;
	if(settings->data()->alarms[type].flash!=flash) {
		settings->data()->alarms[type].flash=flash;
		updateflash();
		}
	return true;
	}

extern "C" JNIEXPORT jboolean  JNICALL   fromjava(alarmhasvibration)(JNIEnv *env, jclass cl,jint type) {
	return !settings->data()->alarms[type].novibration;
	}
extern "C" JNIEXPORT jboolean  JNICALL   fromjava(alarmhassound)(JNIEnv *env, jclass cl,jint type) {
	return !settings->data()->alarms[type].nosound;
	}
extern "C" JNIEXPORT jboolean  JNICALL   fromjava(alarmhasflash)(JNIEnv *env, jclass cl,jint type) {
	return settings->data()->alarms[type].flash;
	}
extern "C" JNIEXPORT jshort  JNICALL   fromjava(readalarmduration)(JNIEnv *env, jclass cl,jint type) {
	return settings->data()->alarms[type].duration;
	}

extern "C" JNIEXPORT jboolean  JNICALL   fromjava(getalarmdisturb)(JNIEnv *env, jclass cl,jint type) {
	return settings->data()->alarms[type].disturb;
	}
extern "C" JNIEXPORT void  JNICALL   fromjava(setalarmdisturb)(JNIEnv *env, jclass cl,jint type,jboolean dist) {
	settings->data()->alarms[type].disturb=dist;
	}
extern "C" JNIEXPORT void  JNICALL   fromjava(writealarmduration)(JNIEnv *env, jclass cl,jint type,jshort dur) {
	settings->data()->alarms[type].duration=dur;
	}
extern "C" JNIEXPORT jshort  JNICALL   fromjava(readalarmsuspension)(JNIEnv *env, jclass cl,jint type) {
	return settings->data()->alarms[type].wait;
	}

extern "C" JNIEXPORT void  JNICALL   fromjava(writealarmsuspension)(JNIEnv *env, jclass cl,jint type,jshort sus) {
	settings->data()->alarms[type].wait=sus;
	}
extern bool fixatex,fixatey;
bool fixatex=true,fixatey=true;
extern "C" JNIEXPORT jboolean  JNICALL   fromjava(getfixatex)(JNIEnv *env, jclass cl) {
	return settings->data()->fixatex;
	}

extern "C" JNIEXPORT void  JNICALL   fromjava(setfixatex)(JNIEnv *env, jclass cl,jboolean val) {
	settings->data()->fixatex=val;

extern int duration;
	fixatex=val;
	settings->data()->duration=duration;
	}
extern "C" JNIEXPORT jboolean  JNICALL   fromjava(getfixatey)(JNIEnv *env, jclass cl) {
	return settings->data()->fixatey;
	}
extern "C" JNIEXPORT void  JNICALL   fromjava(setfixatey)(JNIEnv *env, jclass cl,jboolean val) {
	settings->data()->fixatey=val;
	fixatey=val;
	}
extern int carbotype;
extern "C" JNIEXPORT void  JNICALL   fromjava(setmealvar)(JNIEnv *env, jclass cl,jbyte val) {
     carbotype=val;
	settings->data()->mealvar=val;
	settings->updated();
	}
 
extern "C" JNIEXPORT jbyte  JNICALL   fromjava(getmealvar)(JNIEnv *env, jclass cl) {
	return settings->data()->mealvar;
	}

extern "C" JNIEXPORT jfloat  JNICALL   fromjava(getroundto)(JNIEnv *env, jclass cl) {
	return settings->data()->roundto;
	}
extern "C" JNIEXPORT void  JNICALL   fromjava(setroundto)(JNIEnv *env, jclass cl,jfloat val) {
	settings->data()->roundto=val;
	}

#include "hex.h"
extern "C" JNIEXPORT jboolean  JNICALL   fromjava(setgarminid)(JNIEnv *env, jclass cl,jstring jid) {
	if(jid==nullptr) {
		settings->data()->setdefault();
		return true;
		}
      const char *id = env->GetStringUTFChars( jid, NULL);
        if (id == nullptr) return false;
        destruct   dest([jid,id,env]() {env->ReleaseStringUTFChars(jid, id);});
	uint8_t *nid=settings->data()->watchid;
	for(int i=0;i<16;i++) {
		char h=id[i*2];
		if(!isxdigit(h))
			return false;
		char l=id[i*2+1];
		if(!isxdigit(l))
			return false;
		nid[i]=hexnum(h,l);
		}
	return true;
	}
jstring gethexstring(JNIEnv *env,const uint8_t *nid ) {
	char hexstr[33];
	int uit=0;
	for(int i=0;i<16;i++) {
		hexstr[uit++]=showhex<1>(nid[i]);
		hexstr[uit++]=showhex<0>(nid[i]);
		}
	hexstr[32]='\0';
	 return env->NewStringUTF(hexstr);
	 }
extern "C" JNIEXPORT jstring  JNICALL   fromjava(getdefaultid)(JNIEnv *env, jclass cl) {
	return gethexstring(env,defaultid);
	}
extern "C" JNIEXPORT jstring  JNICALL   fromjava(getgarminid)(JNIEnv *env, jclass cl) {
	return gethexstring(env,settings->data()->watchid);
	}


extern "C" JNIEXPORT jboolean  JNICALL   fromjava(getwaitwithenablestreaming)(JNIEnv *env, jclass cl) {
	return  waitstreaming(); 
	}
extern "C" JNIEXPORT void  JNICALL   fromjava(setwaitwithenablestreaming)(JNIEnv *env, jclass cl,jboolean val) {
	settings->data()->waitwithstreaming=val;
	}
extern "C" JNIEXPORT jboolean  JNICALL   fromjava(nfcsound)(JNIEnv *env, jclass cl) {
	return settings->data()->nfcsound;
	}

extern "C" JNIEXPORT void  JNICALL   fromjava(setnfcsound)(JNIEnv *env, jclass cl,jboolean val) {
	settings->data()->nfcsound=val;
	}

extern "C" JNIEXPORT jboolean  JNICALL   fromjava(getlibrelinkused)(JNIEnv *env, jclass cl) {
	return settings->data()->librelinkBroadcast.nr;
	}
	/*
extern "C" JNIEXPORT void  JNICALL   fromjava(setxinfuus)(JNIEnv *env, jclass cl,jboolean val) {
	settings->data()->xinfuus=val;
	} */

	/*
extern "C" JNIEXPORT void  JNICALL   fromjava(setxbroadcast)(JNIEnv *env, jclass cl,jboolean val) {
	settings->data()->xdripbroadcast=val;
	} */



extern "C" JNIEXPORT jboolean  JNICALL   fromjava(getlevelleft)(JNIEnv *env, jclass cl) {
	return settings->data()->levelleft;
	}

extern "C" JNIEXPORT void  JNICALL   fromjava(setlevelleft)(JNIEnv *env, jclass cl,jboolean val) {
	settings->data()->levelleft=val;
	}

	extern void wakeaftermin(int min);
extern "C" JNIEXPORT void  JNICALL   fromjava(maintenance)(JNIEnv *env, jclass cl) {
	if(settings&&sensors) {
	/*
#ifndef WEAROS
		if(settings->data()->uselibre) {
			wakeaftermin(60);
			}
#endif */
		std::thread removethread(&Sensoren::removeoldstates,sensors);
		removethread.detach();
		}
	
	}

extern "C" JNIEXPORT jboolean  JNICALL   fromjava(getusegarmin)(JNIEnv *env, jclass cl) {
	return settings->data()->usegarmin;
	}
extern "C" JNIEXPORT void  JNICALL   fromjava(setusegarmin)(JNIEnv *env, jclass cl,jboolean val) {
	settings->data()->usegarmin=val;
	}
extern "C" JNIEXPORT jboolean  JNICALL   fromjava(getusexdripwebserver)(JNIEnv *env, jclass cl) {
	return settings->data()->usexdripwebserver;
	}
extern void stopwatchthread() ;
extern	void startwatchthread() ;
extern "C" JNIEXPORT void  JNICALL   fromjava(setusexdripwebserver)(JNIEnv *env, jclass cl,jboolean val) {
#ifndef WEAROS
	settings->data()->usexdripwebserver=val;
	if(val) {
		startwatchthread();
		}
	else {
		stopwatchthread(); 
		}
#endif
	}



extern "C" JNIEXPORT jboolean  JNICALL   fromjava(getuseWearos)(JNIEnv *env, jclass cl) {
	return settings->data()->useWearos;
	}
extern "C" JNIEXPORT void  JNICALL   fromjava(setuseWearos)(JNIEnv *env, jclass cl,jboolean val) {
	settings->data()->useWearos=val;
	}


extern "C" JNIEXPORT void  JNICALL   fromjava(setlibrebaseurl)(JNIEnv *env, jclass cl,jboolean libre3,jstring jbaseurl) {
	char *url=libre3?settings->data()->libre3baseurl:settings->data()->librebaseurl;
	const jint jlen = env->GetStringLength(jbaseurl);
	env->GetStringUTFRegion(jbaseurl, 0,jlen, url);
	jint len = env->GetStringUTFLength( jbaseurl);
	url[len]='\0';
	 }
extern "C" JNIEXPORT jstring  JNICALL   fromjava(getlibrebaseurl)(JNIEnv *env, jclass cl,jboolean libre3) {
	const char *url=libre3?settings->data()->libre3baseurl:settings->data()->librebaseurl;
	 return env->NewStringUTF(url);
	}

extern "C" JNIEXPORT void  JNICALL   fromjava(setlibreemail)(JNIEnv *env, jclass cl,jstring jemail) {
	const jint jlen = env->GetStringLength(jemail);
	env->GetStringUTFRegion(jemail, 0,jlen, settings->data()->libreemail);
	jint len = env->GetStringUTFLength( jemail);
	settings->data()->libreemail[len]='\0';
	 }
extern "C" JNIEXPORT jstring  JNICALL   fromjava(getlibreemail)(JNIEnv *env, jclass cl) {
	 return env->NewStringUTF(settings->data()->libreemail);
	}
#include "mixpass.h"
extern "C" JNIEXPORT void  JNICALL   fromjava(setlibrepass)(JNIEnv *env, jclass cl,jstring jpass) {
	const jint jlen = env->GetStringLength(jpass);
	char tmp[37];
	env->GetStringUTFRegion(jpass, 0,jlen, tmp);
	jint len = env->GetStringUTFLength( jpass);
	mix(mixpass, reinterpret_cast<uint8_t *>(tmp),
		reinterpret_cast<uint8_t *>(settings->data()->librepass), len);
	settings->data()->librepasslen=len;
	 }

bool savefield(JNIEnv *env,jstring jin,uint8_t *out) {
	const jint jlen = env->GetStringLength(jin);
	jint len = env->GetStringUTFLength( jin);
	char tmp[len+1];
	env->GetStringUTFRegion(jin, 0,jlen, tmp);
	tmp[len]='\0';
	mix(mixpass, reinterpret_cast<uint8_t *>(tmp), out, len+1);
	return true;
	}
jstring getfield(JNIEnv *env,uint8_t *in,int len) {
	int totlen=len+1;
	char tmp[totlen];		
	mix(mixpass,in ,reinterpret_cast<uint8_t*>( tmp), totlen);
	return env->NewStringUTF(tmp);
	}
extern "C" JNIEXPORT void  JNICALL   fromjava(savelibrerubbish)(JNIEnv *env, jclass cl,jstring FirstName,jstring LastName,jint dat,jstring GuardianFirstName,jstring GuardianLastName) {
	settings->data()->DateOfBirth=dat;
	savefield(env,FirstName, settings->data()->FirstName);
	savefield(env,LastName, settings->data()->LastName);
	savefield(env,GuardianFirstName, settings->data()->GuardianFirstName);
	savefield(env,GuardianLastName, settings->data()->GuardianLastName);
	}

	/*
std::array<char,sizeof(settings->data->getLastName)>getLastName() {
	std::array<char,sizeof(settings->data->getLastName)> name;
	mix(mixpass, settings->data->getLastName,reinterpret_cast<uint8_t*>( name.data()), name.size());
	return name;
	}
std::array<char,sizeof(settings->data->getFirstName)>getFirstName() {
	std::array<char,sizeof(settings->data->getFirstName)> name;
	mix(mixpass, settings->data->getFirstName,reinterpret_cast<uint8_t*>( name.data()), name.size());
	return name;
	}
std::array<char,sizeof(settings->data->getGuardianFirstName)>getGuardianFirstName() {
	std::array<char,sizeof(settings->data->getGuardianFirstName)> name;
	mix(mixpass, settings->data->getGuardianFirstName,reinterpret_cast<uint8_t*>( name.data()), name.size());
	return name;
	}
std::array<char,sizeof(settings->data->getGuardianLastName)>getGuardianLastName() {
	std::array<char,sizeof(settings->data->getGuardianLastName)> name;
	mix(mixpass, settings->data->getGuardianLastName,reinterpret_cast<uint8_t*>( name.data()), name.size());
	return name;
	}
extern "C" JNIEXPORT jint JNICALL   fromjava(getDateOfBirth)(JNIEnv *env, jclass cl) { return ->data()->DateOfBirth;}
extern "C" JNIEXPORT jstring  JNICALL   fromjava(getLastName)(JNIEnv *env, jclass cl) { return getfield(env, settings->data()->LastName, sizeof(settings->data()->LastName)); }
extern "C" JNIEXPORT jstring  JNICALL   fromjava(getFirstName)(JNIEnv *env, jclass cl) { return getfield(env, settings->data()->FirstName, sizeof(settings->data()->FirstName)); }
extern "C" JNIEXPORT jstring  JNICALL   fromjava(getGuardianFirstNameName)(JNIEnv *env, jclass cl) { return getfield(env, settings->data()->GuardianFirstNameName, sizeof(settings->data()->GuardianFirstNameName)); }
extern "C" JNIEXPORT jstring  JNICALL   fromjava(getGuardianLastNameName)(JNIEnv *env, jclass cl) { return getfield(env, settings->data()->GuardianLastNameName, sizeof(settings->data()->GuardianLastNameName)); }

*/
extern "C" JNIEXPORT jstring  JNICALL   fromjava(getlibrepass)(JNIEnv *env, jclass cl) {
	if(!settings->data()->librepasslen)
		return nullptr;
	const int len=settings->data()->librepasslen;
	char tmp[37];
	mix(mixpass,reinterpret_cast<uint8_t *>(settings->data()->librepass),reinterpret_cast<uint8_t *>(tmp),len);
	tmp[len]='\0';
	 return env->NewStringUTF(tmp);
	}



extern "C" JNIEXPORT void  JNICALL   fromjava(setlibreDeviceID)(JNIEnv *env, jclass cl,jboolean libre3,jstring jDeviceID) {
	auto &deviceID= libre3?settings->data()->libre3viewDeviceID:settings->data()->libreviewDeviceID;
	const jint jlen = env->GetStringLength(jDeviceID);
	env->GetStringUTFRegion(jDeviceID, 0,jlen, deviceID.data());
	deviceID.data()[deviceID.size()]='\0';
	 }
extern "C" JNIEXPORT jstring  JNICALL   fromjava(getlibreDeviceID)(JNIEnv *env, jclass cl,jboolean libre3) {
	const auto &deviceID= libre3?settings->data()->libre3viewDeviceID:settings->data()->libreviewDeviceID;
	 return env->NewStringUTF(deviceID.data());
	}
extern "C" JNIEXPORT void  JNICALL   fromjava(setlibreAccountID)(JNIEnv *env, jclass cl,jstring jAccountID) {
	const jint jlen = env->GetStringLength(jAccountID);
	 auto &accountid=settings->data()->libreviewAccountID;
	 accountid={};
	env->GetStringUTFRegion(jAccountID, 0,jlen, accountid.data());
	settings->data()->_nullchar1='\0';
	 }
extern "C" JNIEXPORT jstring  JNICALL   fromjava(getlibreAccountID)(JNIEnv *env, jclass cl) {
	 return env->NewStringUTF(settings->data()->libreviewAccountID.data());
	}


extern "C" JNIEXPORT void  JNICALL   fromjava(setlibreUserToken)(JNIEnv *env, jclass cl,jboolean libre3,jstring jUserToken) {
	char *usertoken=libre3?settings->data()->libreviewUserToken3:
		settings->data()->libreviewUserToken;
	const jint jlen = env->GetStringLength(jUserToken);
	env->GetStringUTFRegion(jUserToken, 0,jlen, usertoken);
	usertoken[jlen]='\0';
	(libre3?settings->data()->tokensize3:settings->data()->tokensize)=jlen;

	 }
extern "C" JNIEXPORT jstring  JNICALL   fromjava(getlibreUserToken)(JNIEnv *env, jclass cl,jboolean libre3) {
	const char *usertoken=libre3?settings->data()->libreviewUserToken3:settings->data()->libreviewUserToken;
	 return env->NewStringUTF(usertoken);
	}

static jlong getlibreaccountidnumber() {
	 auto &accountid=settings->data()->libreviewAccountID;
	 uint32_t res=0;	
         for(auto el:accountid) {
            res = (res * 0x811C9DC5)^el;
            }

	settings->data()->_nullchar1='\0';
//	*accountid.end()='\0';
	 const jlong lres= static_cast<jlong>(res); 
	 LOGGER("accountID %s %" PRId64 "\n",accountid.data(),lres);
	 return lres;
	 }
extern "C" JNIEXPORT jlong  JNICALL   fromjava(getlibreAccountIDnumber)(JNIEnv *env, jclass cl) {
	 return  getlibreaccountidnumber();
	 }
/*
extern "C" JNIEXPORT jint  JNICALL   fromjava(getlibreAccountIDnumber)(JNIEnv *env, jclass cl) {
	 auto accountid=settings->data()->libreviewAccountID;
	 const int len=accountid.size();
	 const char *str=accountid.data();
	 int res=0;	
         for(int i=0;i<len;i++) {
            res = (res * (-2128831035))^str[i];
            }
	 return res;
	 }


static  private final int h(String str) {
        int length = str.length();
        int i2 = 0;
        int i3 = 0;
        while (i2 < length) {
            char charAt = str.charAt(i2);
            i2++;
            int i4 = i3 * (-2128831035);
            i3 = i4 ^ charAt;
        }
        return i3;
    }

extern "C" JNIEXPORT jint  JNICALL   fromjava(getlastGen)(JNIEnv *env, jclass cl) {
	return getlastGen();
	}
extern "C" JNIEXPORT void  JNICALL   fromjava(setlastGen)(JNIEnv *env, jclass cl,jint val) {
	setlastGen(val);
	}*/






extern "C" JNIEXPORT void  JNICALL   fromjava(setnewYuApiKey)(JNIEnv *env, jclass cl,jboolean libre3,jstring jpass) {
	char *YuKey=libre3?settings->data()->newYuApiKey3:settings->data()->newYuApiKey;
	const jint jlen = env->GetStringLength(jpass);
	env->GetStringUTFRegion(jpass, 0,jlen, YuKey);
	jint len = env->GetStringUTFLength( jpass);
	YuKey[len]='\0';
	 }
extern "C" JNIEXPORT jstring  JNICALL   fromjava(getnewYuApiKey)(JNIEnv *env, jclass cl,jboolean libre3) {
	const char *YuKey=libre3?settings->data()->newYuApiKey3:settings->data()->newYuApiKey;
	 return env->NewStringUTF(YuKey);
	}



extern void startthreads() ;
extern "C" JNIEXPORT void  JNICALL   fromjava(startthreads)(JNIEnv *env, jclass cl) {
	startthreads();
	}

extern int startmeals() ;
extern void	 startsensors() ;

extern void thrownull(JNIEnv * env,const char *message);
extern "C" JNIEXPORT void  JNICALL   fromjava(startmeals)(JNIEnv *env, jclass cl) {
	 switch(startmeals()) {
	 	case 1: thrownull(env,"startmeals settings==null");break;
		case 2: thrownull(env,"startmeals settings->data()==null");break;
		default:break;
	 	}
	 }
extern "C" JNIEXPORT void  JNICALL   fromjava(startsensors)(JNIEnv *env, jclass cl) {
	 startsensors() ;
	 }

extern void initjuggluco(std::string_view dirfiles) ;

extern "C" JNIEXPORT void  JNICALL   fromjava(initjuggluco)(JNIEnv *env, jclass cl,jstring jfilesdir) {
      const char *filesdir = env->GetStringUTFChars( jfilesdir, NULL);
   const  std::string_view::size_type len = env->GetStringUTFLength( jfilesdir);
        if (filesdir == nullptr) return;
        destruct   dest([jfilesdir,filesdir,env]() {env->ReleaseStringUTFChars(jfilesdir, filesdir);});
	initjuggluco({filesdir,len});
	}


extern "C" JNIEXPORT jboolean  JNICALL   fromjava(getpriority)(JNIEnv *env, jclass cl) {
	return !settings->data()->balanced_priority;
	}
extern "C" JNIEXPORT void  JNICALL   fromjava(setpriority)(JNIEnv *env, jclass cl,jboolean val) {
	settings->data()->balanced_priority=!val;
	}

extern bool stopconnection;
extern "C" JNIEXPORT void  JNICALL   fromjava(setXdripServerLocal)(JNIEnv *env, jclass cl,jboolean val) {
#ifndef WEAROS
	const bool remote=!val;
	if(remote!=settings->data()->remotelyxdripserver) {
		settings->data()->remotelyxdripserver=remote;
		stopconnection=true;
		}
#endif
	}
extern "C" JNIEXPORT jboolean  JNICALL   fromjava(getXdripServerLocal)(JNIEnv *env, jclass cl) {
	if(!settings||!settings->data())
		return true;
	return ! settings->data()->remotelyxdripserver;
	}	

extern "C" JNIEXPORT jboolean  JNICALL   fromjava(getShownintro)(JNIEnv *env, jclass cl) {
	return settings->data()->shownintro;
	}
extern "C" JNIEXPORT void  JNICALL   fromjava(setShownintro)(JNIEnv *env, jclass cl,jboolean val) {
	settings->data()->shownintro=val;
	}

extern "C" JNIEXPORT jboolean  JNICALL   fromjava(getkerfstokblack)(JNIEnv *env, jclass cl) {
	return settings->data()->kerfstokblack;
	}
extern "C" JNIEXPORT void  JNICALL   fromjava(setkerfstokblack)(JNIEnv *env, jclass cl,jboolean val) {
	settings->data()->kerfstokblack=val;
	}
#include "inout.h"
bool	getpathworks() {
	pathconcat testprog(globalbasedir,"testprog");
	const char commando[]{"#!/system/bin/sh\ntrue\n"};
	writeall(testprog,commando,sizeof(commando)-1);
	chmod(testprog,0700);
	setenv("PATH",globalbasedir.data(),1);
	int err=system("testprog");
	LOGGER("testprog gives %d\n",err);
	setenv("PATH","/system/bin",1);
	unlink(testprog.data());
	return !err;
	}

extern "C" JNIEXPORT void  JNICALL   fromjava(setlibrenum)(JNIEnv *env, jclass cl,jint index,jint kind, jfloat weight) {
	if(index>=settings->getlabelcount())  {
		return ;
		}
	settings->data()->librenums[index]={kind,weight};
	return ;
	}
extern "C" JNIEXPORT jint  JNICALL   fromjava(getlibrenumkind)(JNIEnv *env, jclass cl,jint index) {
	if(index>=settings->getlabelcount())  {
		return 0;
		}
	return settings->data()->librenums[index].kind;
	}
extern "C" JNIEXPORT jfloat  JNICALL   fromjava(getlibrefoodweight)(JNIEnv *env, jclass cl,jint index) {
	if(index>=settings->getlabelcount())  {
		return 0.0f;
		}
	return settings->data()->librenums[index].weight;
	}

extern "C" JNIEXPORT jboolean  JNICALL   fromjava(getSendNumbers)(JNIEnv *env, jclass cl) {
	return settings->data()->sendnumbers;
	}
extern "C" JNIEXPORT jboolean  JNICALL   fromjava(canSendNumbers)(JNIEnv *env, jclass cl) {
	if(settings->data()->sendnumbers)
		return true;
	const auto *nums= settings->data()->librenums;
	for(int i=0;i<settings->getlabelcount();++i) {
		if(!nums[i].kind) {
			settings->data()->sendnumbers=false;
			return false;
			}
		}
	return settings->getlabelcount()>0;
	}
extern "C" JNIEXPORT void  JNICALL   fromjava(setSendNumbers)(JNIEnv *env, jclass cl,jboolean val) {
	if(settings->data()->sendnumbers!=val) {
		settings->data()->sendnumbers=val;
		sensors->setversions();
		}
	}

extern "C" JNIEXPORT void  JNICALL   fromjava(setwakelock)(JNIEnv *env, jclass cl,jboolean val) {
	settings->data()->wakelock=val;
	}
extern "C" JNIEXPORT jboolean  JNICALL   fromjava(getwakelock)(JNIEnv *env, jclass cl) {
	return settings->data()->wakelock;
	}
extern "C" JNIEXPORT void  JNICALL   fromjava(setheartrate)(JNIEnv *env, jclass cl,jboolean val) {
	settings->data()->heartrate=val;
	}
extern "C" JNIEXPORT jboolean  JNICALL   fromjava(getheartrate)(JNIEnv *env, jclass cl) {
	return settings->data()->heartrate;
	}

jobjectArray  mkjavastringarray(JNIEnv *env,BroadcastListeners<0> *listen) {
        const int len= listen->nr;
        jobjectArray  sensjar = env->NewObjectArray(len,env->FindClass("java/lang/String"),nullptr);
         for(int i=0;i<len;i++) {
                 env->SetObjectArrayElement(sensjar,i,env->NewStringUTF(listen->name[i]));
                  }
	return sensjar;
	}
template <int max> jobjectArray  mkjavastringarray(JNIEnv *env,BroadcastListeners<max> *listen) {
	return  mkjavastringarray(env,reinterpret_cast<BroadcastListeners<0> *>(listen));
	}
void setstringarray(JNIEnv *env,jobjectArray jnames,BroadcastListeners<0> *listen, int maxnr) {
	int len=jnames?env->GetArrayLength(jnames):0;
	int uselen=std::min(maxnr,len);
	for(int i=0;i<uselen;i++) {
		jstring  jname=(jstring)env->GetObjectArrayElement(jnames,i);
		int namelen= env->GetStringUTFLength( jname);
		jint jnamelen = env->GetStringLength( jname);
		char *name=listen->name[i];
		env->GetStringUTFRegion( jname, 0,jnamelen,name ); name[namelen]='\0';
		}
	listen->nr=uselen;
	}


extern "C" JNIEXPORT jobjectArray  JNICALL   fromjava(librelinkRecepters)(JNIEnv *env, jclass cl) {
	return mkjavastringarray(env,&settings->data()->librelinkBroadcast);
        }
extern "C" JNIEXPORT void  JNICALL   fromjava(setlibrelinkRecepters)(JNIEnv *env, jclass cl, jobjectArray jnames) {
	auto *broad=&settings->data()->librelinkBroadcast;
	setstringarray(env,jnames,reinterpret_cast<BroadcastListeners<0> *>(broad), broad->getmax());
        }


extern "C" JNIEXPORT jobjectArray  JNICALL   fromjava(xdripRecepters)(JNIEnv *env, jclass cl) {
	return mkjavastringarray(env,&settings->data()->xdripBroadcast);
        }
extern "C" JNIEXPORT void  JNICALL   fromjava(setxdripRecepters)(JNIEnv *env, jclass cl, jobjectArray jnames) {
	auto *broad=&settings->data()->xdripBroadcast;
	setstringarray(env,jnames,reinterpret_cast<BroadcastListeners<0> *>(broad), broad->getmax());
        }

extern "C" JNIEXPORT jobjectArray  JNICALL   fromjava(glucodataRecepters)(JNIEnv *env, jclass cl) {
	return mkjavastringarray(env,&settings->data()->glucodataBroadcast);
        }
extern "C" JNIEXPORT void  JNICALL   fromjava(setglucodataRecepters)(JNIEnv *env, jclass cl, jobjectArray jnames) {
	auto *broad=&settings->data()->glucodataBroadcast;
	setstringarray(env,jnames,reinterpret_cast<BroadcastListeners<0> *>(broad), broad->getmax());
        }


extern "C" JNIEXPORT jboolean  JNICALL   fromjava(getxbroadcast)(JNIEnv *env, jclass cl) {
	return settings->data()->xdripBroadcast.nr;
	}

extern "C" JNIEXPORT jboolean  JNICALL   fromjava(getJugglucobroadcast)(JNIEnv *env, jclass cl) {
	return settings->data()->glucodataBroadcast.nr;
	}
	/*
extern "C" JNIEXPORT void  JNICALL   fromjava(setJugglucobroadcast)(JNIEnv *env, jclass cl,jboolean val) {
	settings->data()->jugglucobroadcast=val;
	}*/

extern "C" JNIEXPORT void  JNICALL   fromjava(setinitVersion)(JNIEnv *env, jclass cl,jint val) {
	settings->data()->initVersion=val;
	}
extern "C" JNIEXPORT jint  JNICALL   fromjava(getinitVersion)(JNIEnv *env, jclass cl) {
	return settings->data()->initVersion;
	}
