#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#define  ll long long
using  namespace std;
struct Alumno{
    char codigo[5];
    char nombre[11];
    char carrera[15];
    char apellido[20];
    int ciclo;
    float mensualidad;


    friend ostream & operator<< (ostream & stream, Alumno & record)
    {
        stream.write((char*)&record, sizeof(Alumno));
        stream << endl;
        stream << flush;
        return stream;
    }

    friend istream & operator >> (istream & stream, Alumno & record)
    {
        stream.read((char*)&record, sizeof(Alumno));
        stream.get(); //read \n
        return stream;
    }
};

struct free_list{
    Alumno record;
    free_list* next;
};


enum Type{MOVE_TO_LAST , FREE_LIST};
void file_creator(string filename){

    fstream comprobation_file(filename, ios::in|ios::binary);
    if(comprobation_file)  {

        comprobation_file.close();

    }
    else {
        cout<<"Not found ..."<<endl;
        comprobation_file.close();
        fstream create(filename , ios::out|ios::binary);
        create.close();

    }
}
class FixedRecord{
private:
    vector<Alumno> alumnos;
    string file_name;
    Type strategy;
    long long n;
    long long head;
    long long head_size;

public:
    FixedRecord(const string &fileName, Type strategy)
            : file_name(fileName), strategy(strategy) {

        file_creator(file_name); // create file if it doesnt exist yet
        fstream test_file(file_name , ios::binary|ios::in);
        test_file.seekg(0,ios::end);

        bool empty = (test_file.tellg()==0);
        cout<<boolalpha<<test_file.tellg()<<endl;
        test_file.close();
        ll size;
        n=0;
        head_size=0;
        if(strategy==FREE_LIST)
        {   head_size= sizeof(ll);
            if(empty){ // n should remain as 0 and head must be setted up
                fstream set_file_free(fileName , ios::binary|ios::out);
                ll initializer=-1;
                set_file_free.seekp(0, ios::beg);
                set_file_free.write((char*)&initializer, head_size );
                head=-1;
                set_file_free.close();
            }else{ // calculate n and extract head from header
                fstream set_file_free(fileName , ios::binary|ios::in);
                set_file_free.seekg(0, ios::beg);
                set_file_free.read((char*)&head , head_size);

                set_file_free.seekg(0, ios::end);
                ll byte_size=set_file_free.tellg();
                n = (byte_size-head_size)/(sizeof(Alumno)+head_size);
                set_file_free.close();
            }
        }

        if(strategy==MOVE_TO_LAST)
        {
            if(empty){

            }else{

            }
        }





    }

    vector<Alumno> load(){

        fstream read_file(file_name , ios::in|ios::binary);
        if(!read_file)
        {
            cerr<<"No se encontro el archivo...";
            exit(1);
        }
        Alumno prov;
        if(strategy==FREE_LIST){
            ll register_size = sizeof(Alumno) + head_size;
            read_file.seekg(head_size, ios::beg);
            ll pointer;
            for (int i = 0; i < n; ++i) {

                read_file.seekg(i*register_size+head_size);
                read_file.read((char*)&prov , sizeof(Alumno));
                read_file.read((char*)& pointer , head_size);
                if(pointer==-2) {
                    alumnos.push_back(prov);

                }
            }
        }else {
            ll size = sizeof(Alumno);

            for (int i = 0; i < n; ++i) {
                read_file.seekg(i*size);
                read_file.read((char*)&prov , sizeof(Alumno));
                cout << prov.nombre << endl;
                alumnos.push_back(prov);

            }


        }
        read_file.close();
        return alumnos;
    }

    Alumno readRecord( int pos){
//TODO read record para cada tipo de dato
        fstream read_file(file_name , ios::in|ios::binary);
        if(!read_file)
        {
            cerr<<"No se encontro el archivo...";
            exit(1);
        }
        read_file.seekg(0, ios::beg);
        Alumno prov{};
        read_file.seekg(pos*(sizeof (Alumno)+head_size));
        read_file.read((char*)&prov , sizeof (Alumno));

        return prov;
    }

    void add(Alumno record){

        if(strategy==FREE_LIST)
        {
            fstream write_file(file_name , ios::out|ios::in |ios::binary);
            if(!write_file)
            {
                cerr<<"No se encontro el archivo...";
                exit(1);
            }
            write_file.seekp(head_size,ios::beg);
            write_file.seekg(0, ios::beg);
            ll cabeza;
            write_file.read((char*)&cabeza , head_size);
            cout<<"EL HEAD LEIDO ES "<<cabeza<<endl;
            if(head==-1)
            {

                write_file.seekp(0,ios::end);
                ll index=-2;
                write_file.write((char*)&record, sizeof(Alumno));
                write_file.write((char*)&index , head_size);
                n++;
                cout<<n<<endl;

            }else {
                int size = sizeof(Alumno)+head_size ;
                write_file.seekg(head*size + sizeof(Alumno)+head_size);
                cout<<"NEW TELL G "<<write_file.tellg()<<endl;
                ll temp;
                write_file.read((char*)&temp , head_size);
                cout<<"MY TEMP ES "<<temp<<endl;
                write_file.seekp(head*(sizeof(Alumno)+head_size)+head_size);
                ll index=-2;
                write_file.write((char*)&record , sizeof(Alumno));
                write_file.write((char*)&index , head_size);


                head=temp;
                write_file.seekp(0 , ios::beg);
                write_file.write((char*)&head , head_size);

            }

        }else{

            n++;
            fstream read_file(file_name , ios::out  |ios::app |ios::binary);
            if(!read_file)
            {
                cerr<<"No se encontro el archivo...";
                exit(1);
            }

            int size = sizeof(Alumno);
            read_file.seekg(0, ios::beg);
            read_file.write((char*)&record , sizeof(Alumno));
            read_file.close();

        }

    }

    int getHead() const {
        return head;
    }


    void Delete(int pos){

        fstream read_file(file_name , ios::in|ios::binary  |ios::out);
        if(!read_file)
        {
            cerr<<"No se encontro el archivo...";
            exit(1);
        }

        int size = sizeof(Alumno)+head_size ;

        read_file.seekg(head_size, ios::beg);
        if(strategy==FREE_LIST)
        {
            int temp=head;
            head=pos;
            read_file.seekp(0,ios::beg);
            read_file.write((char*)&head, head_size);
            read_file.seekp(pos*size + sizeof(Alumno)+head_size);
            read_file.write((char*)&temp , head_size);


        }else{

            Alumno temp2;
            read_file.seekg((n-1)*size);
            read_file.read((char*)&temp2 , size); // saco el final
            cout<<"A BORRAR"<<temp2.nombre<<endl;
            read_file.seekp(ios::beg); //posiciono el puntero de escriture en el inicio
            read_file.seekp(pos*size);//lo muevo a la posicion para escribir
            cout<<read_file.tellp()<<endl;
            read_file.write((char*)&temp2 , size); // escribimos en la posicion
            n--;
        }
        read_file.close();
    }



};
void show_all(string filename ){
    FixedRecord file(filename,FREE_LIST);
    vector<Alumno> all_students = file.load();
    cout << "----------------------------" << endl;
    for (Alumno al: all_students)
        cout << al.nombre << endl;
}

void testadd (Type file_type ) {

    Alumno al1 {"2312", "Jose","Lopez","CS",5,123.12};
    Alumno al2 {"2311", "Juana","Rosa","DS",4,1422.2};
    Alumno al3 {"2202", "Fabricio","Chavez","CS",6,300.99};
    Alumno al4 {"2020", "Jesus","UWU","CS",8,9.999};
    Alumno al5 {"1999", "Juancito","OWO","CS",10,1999.999};
    string file_name="ejemplo1.bin";
    FixedRecord fr(file_name , file_type);
    fr.add(al1);
    fr.add(al2);
    fr.add(al3);
    fr.add(al4);
    fr.add(al5);
    show_all(file_name);
    fr.Delete(0);
    fr.Delete(4);
    show_all(file_name);




//    fr.Delete(2);
//    cout<<"After delete ..."<<endl;
//    show_all(file_name);




}

int main() {

    testadd(FREE_LIST);

    return 0;


}