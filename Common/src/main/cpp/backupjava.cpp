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
/*      Fri Jan 27 12:35:35 CET 2023                                                 */


#include <jni.h>
#include <alloca.h>
#include "fromjava.h"
#include "datbackup.h"
#include "net/netstuff.h"


void netwakeup();
extern bool networkpresent;

extern "C" JNIEXPORT jboolean  JNICALL   fromjava(backuphasrestore)(JNIEnv *env, jclass cl) {
	return backup->getupdatedata()->hasrestore;
	}
extern "C" JNIEXPORT jint  JNICALL   fromjava(backuphostNr)(JNIEnv *env, jclass cl) {
	return backup->	gethostnr();
	}
//static native String[] getbackuphostnames(int pos);

extern "C" JNIEXPORT jboolean  JNICALL   fromjava(detectIP)(JNIEnv *envin, jclass cl,jint pos) {
	const passhost_t &host=backup->getupdatedata()->allhosts[pos];
	return host.detect;
	}
extern "C" JNIEXPORT jobjectArray  JNICALL   fromjava(getbackuphostnames)(JNIEnv *env, jclass cl,jint pos) {
	if(!backup)  {
		LOGGER("backup==null\n");
		return nullptr;
		}
	const auto hostnr=backup->gethostnr();
	if(pos>=hostnr) {
		LOGGER("pos(%d)>=backup->gethostnr()(%d)\n",pos,hostnr);
		return nullptr;
		}
	passhost_t &host=backup->getupdatedata()->allhosts[pos];
	const int len=host.nr;
	if(len<0) {
		LOGGER("host.nr==%d\n",len);
		host.nr=0;
		}
	jobjectArray  ipar = env->NewObjectArray(len,env->FindClass("java/lang/String"),nullptr);
	if(!ipar) {
		LOGGER(R"(NewObjectArray(%d,env->FindClass("java/lang/String")==null)""\n",len);
		return nullptr;
		}
	for(int i=0;i<len;i++) {
		namehost name(host.ips+i);
		env->SetObjectArrayElement(ipar,i,env->NewStringUTF(name));
		}
	return ipar;
	}

/*
extern "C" JNIEXPORT jstring JNICALL   fromjava(getbackuphostname)(JNIEnv *envin, jclass cl,jint pos) {
	if(address(backup->getupdatedata()->allhosts[pos])) {
		auto host=backup->gethost(pos);
		return envin->NewStringUTF(host);
		}
	return nullptr;
	}
	*/


int getposbylabel(const char *label) {
	const int nr=backup->gethostnr();
	for(int pos=0;pos<nr;++pos) {
		const passhost_t &host=backup->getupdatedata()->allhosts[pos];
		if(!host.hasname)
		   continue;
		if(!strcmp(host.getname(),label)) {
		    LOGGER("getposbylabel(%s)=%d\n",label,pos);
		    return pos;
		    }
	 	}
    LOGGER("getposbylabel(%s)=-1\n",label);
    return -1;
	}
bool removebylabel(const char *label) {
	int pos = getposbylabel(label);
	if (pos < 0)
		return false;
	backup->deletehost(pos);
	return true;
	}
/*
extern "C" JNIEXPORT jboolean JNICALL   fromjava(removebylabel)(JNIEnv *env, jclass cl,jstring jlabel) {
      const char *label = env->GetStringUTFChars( jlabel, NULL);
        if(!label) return false;
        destruct   dest([jlabel,label,env]() {env->ReleaseStringUTFChars(jlabel, label);});
	return removebylabel(label);
	} */

#ifndef ABBOTT
passhost_t * getwearoshost(const bool create,const char *label);
bool resetbylabel(const char *label) {
	int pos=getposbylabel(label);
	if(pos<0)
		return false;
	const passhost_t &host=backup->getupdatedata()->allhosts[pos];
	const int nr=host.nr;
	if(nr>0) {
		struct sockaddr_in6 ips[host.nr];
		memcpy(ips,host.ips,sizeof(ips));
		backup->deletehost(pos);
		passhost_t *newhost=getwearoshost(true,label);
		memcpy(newhost->ips,ips,sizeof(ips));
		newhost->nr=nr;
		}
	else {
		backup->deletehost(pos);
		}
	return true;
	}
	
extern "C" JNIEXPORT jboolean JNICALL   fromjava(resetbylabel)(JNIEnv *env, jclass cl,jstring jlabel) {
      const char *label = env->GetStringUTFChars( jlabel, NULL);
        if(!label) return false;
        destruct   dest([jlabel,label,env]() {env->ReleaseStringUTFChars(jlabel, label);});
	return resetbylabel(label);
	}
#endif
const char *gethostlabel(int pos) {
	if(!backup||pos>=backup->gethostnr())
		return nullptr;
	const passhost_t &host=backup->getupdatedata()->allhosts[pos];
	if(!host.hasname)
		return nullptr;
	return host.getname();
	}
bool gethosttestip(int pos) {
	if(!backup||pos>=backup->gethostnr())
		return true;
	const passhost_t &host=backup->getupdatedata()->allhosts[pos];
	return !host.noip;
	}
extern "C" JNIEXPORT jboolean JNICALL   fromjava(getbackuptestip)(JNIEnv *envin, jclass cl,jint pos) {
	return gethosttestip(pos);
	}
extern "C" JNIEXPORT jstring JNICALL   fromjava(getbackuplabel)(JNIEnv *envin, jclass cl,jint pos) {
	if(const char *label=gethostlabel(pos))
		return envin->NewStringUTF(label);
	return nullptr;
	}
extern "C" JNIEXPORT jstring JNICALL   fromjava(getbackuppassword)(JNIEnv *envin, jclass cl,jint pos) {
	if(!backup||pos>=backup->gethostnr())
		return nullptr;
	return envin->NewStringUTF(backup->getpass(pos).data());
	}
extern "C" JNIEXPORT jstring JNICALL   fromjava(getbackuphostport)(JNIEnv *envin, jclass cl,jint pos) {
	if(!backup||pos>=backup->gethostnr())
		return nullptr;
	char port[6];
	backup->getport(pos,port);
	return envin->NewStringUTF(port);
	}
extern "C" JNIEXPORT jboolean JNICALL   fromjava(isWearOS)(JNIEnv *envin, jclass cl,jint pos) {
	if(!backup||pos<0||pos>=backup->gethostnr()) {
		LOGGER("isWearos(%d)=false\n",pos);
		return false;
		}
	auto ret= backup->getupdatedata()->allhosts[pos].wearos;
	LOGGER("isWearos(%d)=%d\n",pos,ret);
	return ret;
	}


bool getpassive(int pos);
bool getactive(int pos); 
extern "C" JNIEXPORT jboolean JNICALL   fromjava(getbackuphostactive)(JNIEnv *envin, jclass cl,jint pos) {
	bool active=getactive(pos);
	LOGGER("getbackuphostactive(%d)=%d\n",pos,active);
	return active;
	}

extern "C" JNIEXPORT jboolean JNICALL   fromjava(getbackuphostpassive)(JNIEnv *envin, jclass cl,jint pos) {
	return getpassive(pos);
	}
extern "C" JNIEXPORT int JNICALL   fromjava(getbackuphostreceive)(JNIEnv *envin, jclass cl,jint pos) {
	if(pos<backup->getupdatedata()->hostnr) 
		return backup->getupdatedata()->allhosts[pos].receivefrom;
	return 0;
	}
extern "C" JNIEXPORT jboolean JNICALL   fromjava(getbackuphostnums)(JNIEnv *envin, jclass cl,jint pos) {
	if(pos<backup->getupdatedata()->hostnr) {
		int index=backup->getupdatedata()->allhosts[pos].index;
		if(index>=0)
			return  backup->getupdatedata()->tosend[index].sendnums;
		}
	return false;
	}
extern "C" JNIEXPORT jboolean JNICALL   fromjava(getbackuphoststream)(JNIEnv *envin, jclass cl,jint pos) {
	if(pos<backup->getupdatedata()->hostnr) {
		int index=backup->getupdatedata()->allhosts[pos].index;
		if(index>=0)
			return  backup->getupdatedata()->tosend[index].sendstream;
		}
	return false;
	}
extern "C" JNIEXPORT jboolean JNICALL   fromjava(getbackuphostscans)(JNIEnv *envin, jclass cl,jint pos) {
	if(pos<backup->getupdatedata()->hostnr) {
		int index=backup->getupdatedata()->allhosts[pos].index;
		if(index>=0)
			return  backup->getupdatedata()->tosend[index].sendscans;
		}
	return false;
	}
extern "C" JNIEXPORT void JNICALL   fromjava(setreceiveport)(JNIEnv *env, jclass cl,jstring jport) {
	jint portlen= env->GetStringUTFLength( jport);
	if(portlen<6) {
		char newport[portlen+1];
		jint jlen = env->GetStringLength( jport);
	 	env->GetStringUTFRegion( jport, 0,jlen, newport); 
		if(backup->getupdatedata()->port[portlen]||memcmp(newport, backup->getupdatedata()->port,portlen)) {
			memcpy(backup->getupdatedata()->port,newport,portlen);
			backup->getupdatedata()->port[portlen]='\0';
	//		backup->stopreceiver();
			backup->startreceiver(true);
			}
		}
	}
extern "C" JNIEXPORT jstring JNICALL   fromjava(getreceiveport)(JNIEnv *env, jclass cl) {
	return env->NewStringUTF(backup->getupdatedata()->port);
	}
/*
extern "C" JNIEXPORT jboolean JNICALL   fromjava(stringarray)(JNIEnv *env, jclass cl,jobjectArray jar ) {
	constexpr const int maxad=4;	
	int len=env->GetArrayLength(jar);
	LOGGER("stringarray ");
	const char port[]="8795";
	struct sockaddr_in6 	connect[maxad];
	int uselen=std::min(maxad,len);
	for(int i=0;i<uselen;i++) {
		jstring  jname=(jstring)env->GetObjectArrayElement(jar,i);
		int namelen= env->GetStringUTFLength( jname);
		char name[namelen+1];
		env->GetStringUTFRegion( jname, 0,namelen, name); name[namelen]='\0';
		LOGGER("%s ",name);
		if(!getaddr(name,port,connect+i))
			return  false;
		}
	LOGGER("\nips: ");
	for(int i=0;i<uselen;i++) {
		LOGGER("%s ",namehost(connect+i));
		}
	LOGGER("\n");
	return true;
	}
	*/

//extern "C" JNIEXPORT jint JNICALL   fromjava(changebackuphost)(JNIEnv *env, jclass cl,jint pos,jobjectArray jnames,jint nr,jboolean detect,jstring jport,jboolean nums,jboolean stream,jboolean scans,jboolean recover,jboolean receive,jboolean reconnect,jboolean accepts,jstring jpass,jlong starttime) {
//extern bool mkwearos;
#include <mutex>
extern std::mutex change_host_mutex;
extern "C" JNIEXPORT jint JNICALL   fromjava(changebackuphost)(JNIEnv *env, jclass cl,jint pos,jobjectArray jnames,jint nr,jboolean detect,jstring jport,jboolean nums,jboolean stream,jboolean scans,jboolean recover,jboolean receive,jboolean activeonly,jboolean passiveonly,jstring jpass,jlong starttime,jstring jlabel,jboolean testip) {
  const std::lock_guard<std::mutex> lock(change_host_mutex);
	jint portlen= env->GetStringUTFLength( jport);
	jint jlen = env->GetStringLength( jport);
	char port[portlen+1]; env->GetStringUTFRegion( jport, 0,jlen, port); port[portlen]='\0';
	char *passptr=nullptr;
	jint passlen=0;
	if(jpass) {
		passlen= env->GetStringUTFLength( jpass);

		jint jpasslen = env->GetStringLength( jpass);
		passptr=(char *)alloca(passlen+1); 
		env->GetStringUTFRegion( jpass, 0,jpasslen, passptr); passptr[passlen]='\0';
		}
	const char *label=jlabel?env->GetStringUTFChars( jlabel, NULL):nullptr;
 	jint res=backup->changehost(pos,env,jnames,nr,detect,std::string_view(port,portlen),nums,stream,scans,recover,receive,activeonly,std::string_view(passptr,passlen),starttime,passiveonly,label,testip);
	if(jlabel)
		env->ReleaseStringUTFChars(jlabel, label);
	return res;
	}
extern "C" JNIEXPORT jboolean JNICALL   fromjava(isreceiving)(JNIEnv *env, jclass cl) {
	return backup->isreceiving() ;
	}
extern "C" JNIEXPORT void JNICALL   fromjava(deletebackuphost)(JNIEnv *env, jclass cl,jint pos) {
	backup->deletehost(pos);
	}
extern "C" JNIEXPORT jlong JNICALL   fromjava(lastuptodate)(JNIEnv *env, jclass cl,jint pos) {
	return lastuptodate[pos]*1000LL;
	}
extern "C" JNIEXPORT void JNICALL   fromjava(setWifi)(JNIEnv *env, jclass cl,jboolean val) {
	settings->data()->keepWifi=val;
	}
extern "C" JNIEXPORT jboolean JNICALL   fromjava(getWifi)(JNIEnv *env, jclass cl) {
	return settings->data()->keepWifi;
	}
extern "C" JNIEXPORT jboolean JNICALL   fromjava(stopWifi)(JNIEnv *env, jclass cl) {
	if(settings->data()->keepWifi)
		return false;
		
	return 	(time(nullptr)-((long)lastuptodate[0]))<2*60;
	}

extern "C" JNIEXPORT void JNICALL   fromjava(resetbackuphost)(JNIEnv *env, jclass cl,jint pos) {
	backup->resethost(pos) ;
	}
extern void wakeaftermin(const int waitmin) ;

extern "C" JNIEXPORT void JNICALL   fromjava(networkpresent)(JNIEnv *env, jclass cl) {
      LOGGER("networkpresent\n");
	if(backup) {
		backup->getupdatedata()->wakesender();
		networkpresent=true;
		backup->notupdatedsettings();
		}
	else
		networkpresent=true;
#ifndef WEAROS	
	 wakeaftermin(0) ;
#endif
	}
void resetnetwork() {
	LOGGER("resetnetwork\n");
	if(backup) {
		backup->closeallsocks();
		backup->getupdatedata()->wakesender();
		networkpresent=true;
		backup->notupdatedsettings();
		}
	}
extern "C" JNIEXPORT void JNICALL   fromjava(resetnetwork)(JNIEnv *env, jclass cl) {
	resetnetwork();
	}

extern "C" JNIEXPORT void JNICALL   fromjava(networkabsent)(JNIEnv *env, jclass cl) {
      LOGGER("networkabsent\n");
	networkpresent=false;
	if(backup) {
		backup->closeallsocks();
		}
	}
extern "C" JNIEXPORT void JNICALL   fromjava(wakestreamsender)(JNIEnv *env, jclass cl) {
	if(backup) {
		backup->getupdatedata()->wakestreamsender();
		}
	}
extern "C" JNIEXPORT void JNICALL   fromjava(wakestreamhereonly)(JNIEnv *env, jclass cl) {
	if(backup) {
		backup->wakebackup(Backup::wakestream);
		}
	}
extern "C" JNIEXPORT void JNICALL   fromjava(wakebackup)(JNIEnv *env, jclass cl) {
	if(backup) {
		backup->getupdatedata()->wakesender();
		backup->wakebackup();
		}
	}
extern "C" JNIEXPORT void JNICALL   fromjava(wakehereonly)(JNIEnv *env, jclass cl) {
	if(backup) {
		backup->wakebackup();
		}
	}
/*
extern "C" JNIEXPORT jobjectArray  JNICALL   fromjava(activeSensors)(JNIEnv *env, jclass cl) {
	setusedsensors();
	const int len= usedsensors.size();
	jobjectArray  sensjar = env->NewObjectArray(len,env->FindClass("java/lang/String"),nullptr);
	 for(int i=0;i<len;i++) {
	 	 int index=usedsensors[i];	
		 env->SetObjectArrayElement(sensjar,i,env->NewStringUTF(sensors->shortsensorname(index)->data()));
		  }

	return sensjar;
	}

*/
/*
#ifndef NOWATCHSERVER
void startwatchthread() ;

extern "C" JNIEXPORT void JNICALL   fromjava(startwatchserver)(JNIEnv *env, jclass cl) {
	 startwatchthread() ;
	 }
extern void stopwatchthread() ;

extern "C" JNIEXPORT void JNICALL   fromjava(stopwatchserver)(JNIEnv *env, jclass cl) {

	 stopwatchthread() ;
	 }
#endif
*/