#include <iostream>
#include <windows.h>
#include <time.h>
#include <cstdlib>
#include <unistd.h>
#include <queue> //libreria per la lista

const int DIM_PISTA=100;
const int MIN_TAGLIE=24;
const int MAX_TAGLIE=44;
const int DIM_PATTINI=6; //pattini per taglia

using namespace std;

class Orario{
	private:
		unsigned int ora;
		unsigned int min;
		unsigned int sec;
		unsigned int day;
	public:
		Orario(){
			day=1;
			ora=min=sec=0;
		}
		void update(unsigned int secondi){//non faccio controlli perch� il parametro lo metto io nel main
			if((sec+secondi)>59){
				sec=((sec+secondi)-60);
				if((min+1)>59){
					min=0;
					if(ora+1>23){
						ora=0;
						day++;
					}
					else
						ora++;
				}else
					min++;
			}else{
				sec+=secondi;
			}
		}
		Orario(const Orario &o){
			ora=o.ora;
			min=o.min;
			sec=o.sec;
			day=o.day;
		}
		friend ostream&operator<<(ostream&out, const Orario&o);
};

ostream&operator<<(ostream&out, const Orario&o){
	out<<"Giorno "<<o.day<<":"<<endl<<endl;
	if(o.ora<10)
		out<<"0"<<o.ora<<":";
	else
		out<<o.ora<<":";
	if(o.min<10)
		out<<"0"<<o.min<<":";
	else
		out<<o.min<<":";
	if(o.sec<10)
		out<<"0"<<o.sec;
	else
		out<<o.sec;
	out<<endl<<endl<<endl;
	return out;	
}
class Pattini{
	private:
		unsigned int id;
		unsigned int taglia;
		bool isDisp;
		unsigned int uses;
	public:
		Pattini(){
			id=0;
			taglia=0;
			isDisp=true;
			uses=0;
		}
		Pattini(unsigned int id,unsigned int taglia,bool isDisp,unsigned int uses){
			this->id=id;
			this->taglia=taglia;
			this->isDisp=isDisp;
			this->uses=uses;
		}
		bool getDisp(){
			return isDisp;
		}
		void setDisp(bool disp){
			isDisp=disp;
		}
		int getTaglia(){
			return taglia;
		}
		unsigned int getId(){
			return id;
		}
		operator++(int){
			uses++;
		}
		Pattini(const Pattini &p){
			id=p.id;
			taglia=p.taglia;
			uses=p.uses;
			isDisp=p.isDisp;
		}
		friend ostream&operator<<(ostream &out, const Pattini&p);
};
ostream&operator<<(ostream &out, const Pattini&p){
	out<<"Pattino "<<p.id<<":"<<endl;
	out<<"\tTaglia: "<<p.taglia<<endl;
	p.isDisp?out<<"\t"<<"Disponibile":out<<"\t"<<"Non disponibile";
	out<<endl<<endl<<endl;
	return out;
}
class Persona{
	private:
		unsigned int id;
		unsigned int idGruppo;
		Orario entrata;
		Pattini p;
		Orario uscita;
		unsigned int taglia;
	public:
		Persona(){
			id=0;
			idGruppo=0;
			entrata=Orario();
			p=Pattini();
			uscita=Orario();
			taglia=0;
		}
		int getTaglia(){
			return taglia;
		}
		void setTaglia(int taglia){
			this->taglia=taglia;
		}
		void setPattini(Pattini p1){
			p=p1;
		}
		void setEntrata(Orario o){
			entrata=o;
		}
		void setUscita(Orario o){
			uscita=o;
		}
		int getPatId(){
			return p.getId();
		}
		void setId(int id){
			this->id=id;
		}
		void setGroupId(int id){
			idGruppo=id;
		}
};

Orario orologio=Orario();
void init(Pattini *);
bool thereIsPat(Pattini *,Persona *,int);
int searchPat(Pattini *,int id);

int main(){
	srand(time(NULL));
	Persona pista[DIM_PISTA]; //pista.
	int last=0;
	int lastId=0;
	queue<Persona *> waitList; //lista d'attesa delle persone che devono entrare.
	Pattini pat[DIM_PATTINI*((MAX_TAGLIE-MIN_TAGLIE)+1)];
	init(pat);
	/*for(int i=0;i<DIM_PATTINI*((MAX_TAGLIE-MIN_TAGLIE)+1);i++)
		cout<<pat[i];*/
	cout<<"ESC - per uscire"<<endl;
	while(!GetAsyncKeyState(VK_ESCAPE)){
		cout<<orologio;
		int nPersGroup=rand()%7+1;
		Persona gruppo[nPersGroup];
		for(int i=0;i<nPersGroup;i++){
			gruppo[i].setId(i);
			gruppo[i].setGroupId(lastId);
			gruppo[i].setTaglia(rand()%(MAX_TAGLIE-MIN_TAGLIE+1)+MIN_TAGLIE); //genero una taglia casuale tra il minimo delle taglie e il massimo e la imposto come valore alla taglia della persona
		}
		lastId++;
		if((nPersGroup+last)<DIM_PISTA){
			if(thereIsPat(pat,gruppo,nPersGroup)){
				for(int i=0;i<nPersGroup;i++){
					pista[last]=gruppo[i];
					last++;
				}
			}else{
				waitList.push(gruppo);
			}
		}else
			waitList.push(gruppo);
		//ogni secondo incremento l'orologio di 15 secondi
		sleep(1);
		orologio.update(15);
	}
	return 0;
}
bool thereIsPat(Pattini *pat,Persona *gruppo,int nPers){
	for(int i=0;i<nPers;i++){
		int j=0;
		bool flag=true;
		while(j<DIM_PATTINI*((MAX_TAGLIE-MIN_TAGLIE)+1) && flag){
			if(gruppo[i].getTaglia()==pat[j].getTaglia() && pat[j].getDisp()){
				pat[j].setDisp(false);
				gruppo[i].setPattini(pat[j]);
				flag=false;
			}
			j++;
		}
		if(flag){
			for(int k=0;k<i;k++){
				if(searchPat(pat,gruppo[k].getPatId())!=-1){
					pat[searchPat(pat,gruppo[k].getPatId())].setDisp(true);
					gruppo[k].setPattini(Pattini());
				}
			}
			return false;
		}
	}
	for(int i=0;i<nPers;i++){
		pat[searchPat(pat,gruppo[i].getPatId())]++;
		int min=rand()%10+1;
		gruppo[i].setEntrata(orologio);
		Orario uscita=orologio; //varibaile per creare l'orario di uscita dalla pista della persona
		uscita.update(60*min);//incremento l'orologio attuale di 60(secondi)*min(minuti) -> secondi => il massimo che puo' stare dentro una persona
		gruppo[i].setUscita(uscita); //imposto l'orario di uscita
	}
	return true;
}
int searchPat(Pattini *pat,int id){
	for(int i=0;i<DIM_PATTINI*((MAX_TAGLIE-MIN_TAGLIE)+1);i++){
		if(pat[i].getId()==id){
			pat[i].setDisp(true);	
			return i;
		}
	}
	return -1;
}
void init(Pattini *pat){
	int taglia=MIN_TAGLIE;
	for(int i=0;i<DIM_PATTINI*((MAX_TAGLIE-MIN_TAGLIE)+1);i++){
		pat[i]=Pattini(i,taglia,true,0);
		if((i+1)%DIM_PATTINI==0)//se i e' divisibile per la dimensione dell'array dei pattini allora incremento la taglia
			taglia++;
	}
}
