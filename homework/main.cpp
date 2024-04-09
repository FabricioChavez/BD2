#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
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
class FixedRecord{
private:
    vector<Alumno> alumnos;
    string file_name;
    Type strategy;
    long long n;
    int head;

public:
    FixedRecord(const string &fileName, Type strategy)
    : file_name(fileName), strategy(strategy) {
        fstream read_file(file_name , ios::out|ios::binary|ios::in);
        if(!read_file)
        {
            cerr<<"No se encontro el archivo...";
            exit(1);
        }

        int size = sizeof(Alumno)+sizeof(int);

        read_file.seekg(0,ios::end);
        cout<<read_file.tellg()<<endl;
        n=read_file.tellg()/size;
        read_file.close();
        head =-1;
    }

    void load(){
//        cout << "LOAD" << endl;
//        cout<<file_name<<n<<endl;
        fstream read_file(file_name , ios::in|ios::binary);
        if(!read_file)
        {
            cerr<<"No se encontro el archivo...";
            exit(1);
        }

        int size = sizeof(Alumno) + sizeof(int);
        read_file.seekg(0, ios::beg);
        Alumno prov{};
        int cosa;
        for (int i = 0; i < n; ++i) {
            read_file.seekg(i*size);
            read_file.read((char*)&prov , sizeof(Alumno));
            read_file.read((char*)& cosa , sizeof(int));
            if(cosa==-2) {
                cout << prov.nombre << endl;
                alumnos.push_back(prov);
            }
        }
        read_file.close();
    }

    Alumno readRecord( int pos){

        fstream read_file(file_name , ios::in|ios::binary);
        if(!read_file)
        {
            cerr<<"No se encontro el archivo...";
            exit(1);
        }
        read_file.seekg(0, ios::beg);
        Alumno prov{};
        read_file.seekg(pos*sizeof (Alumno));
        read_file.read((char*)&prov , sizeof (Alumno));

        return prov;
    }

    void add(Alumno record){

        if(strategy==FREE_LIST)
        {
            fstream write_file(file_name , ios::out |ios::in |ios::binary);
            if(!write_file)
            {
                cerr<<"No se encontro el archivo...";
                exit(1);
            }
            write_file.seekp(0,ios::beg);
            if(head==-1)
            {    write_file.seekp(n*(sizeof(Alumno)+sizeof(int)));
                int index=-2;
                write_file.write((char*)&record, sizeof(Alumno));
                write_file.write((char*)&index , sizeof(int));
                n++;

            }else {
               write_file.seekg(0,ios::beg);
                write_file.seekg(head*(sizeof(Alumno)+sizeof(int)));
                write_file.seekg(sizeof(Alumno));
                int temp;
                write_file.seekp(head*(sizeof(Alumno)+sizeof(int)));
                write_file.read((char*)&temp , sizeof(int));
                int index=-2;
                write_file.write((char*)&record , sizeof(Alumno));
                write_file.write((char*)&index , sizeof(int));
                head=temp;

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

    void print(){
        cout << "PRINT "<<endl;
        ifstream ifile(file_name, ios::in|ios::binary);
        Alumno alumno;

        // Mientras haya datos disponibles para leer, sigue leyendo y mostrando los registros
        int i = 0;
        while (ifile >> alumno && i < n) {
            cout << alumno.nombre << endl;
            i++;
        }

        ifile.close();
    }
    void Delete(int pos){

        fstream read_file(file_name , ios::in|ios::binary  |ios::out);
        if(!read_file)
        {
            cerr<<"No se encontro el archivo...";
            exit(1);
        }

        int size = sizeof(Alumno)+sizeof(int);
        read_file.seekg(0, ios::beg);

        if(strategy==FREE_LIST)
        {
            int temp=head;
            head=pos;
            read_file.seekp(pos*size + sizeof(Alumno));
            read_file.write((char*)&temp , sizeof(int));


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


int main() {
    Alumno al1 {"2312", "Jose","Lopez","CS",5,123.12};
    Alumno al2 {"2311", "Juana","Rosa","DS",4,1422.2};
    Alumno al3 {"2202", "Fabricio","Chavez","CS",6,300.99};
    Alumno al4 {"2020", "Jesus","UWU","CS",8,9.999};
    Alumno al5 {"1999", "Juancito","OWO","CS",10,1999.999};
    string file_name="ejemplo1.bin";
    fstream afuera(file_name , ios::out |ios::binary);
    if(!afuera){
        cerr<<"File not found ...";
        exit(1);
    }
    afuera.seekp(ios::beg);
    int index=-2;
    afuera.write((char*)&al1 , sizeof (Alumno) );
    afuera.write((char*)&index , sizeof (int));
    afuera.write((char*)&al2 , sizeof (Alumno) );
    afuera.write((char*)&index , sizeof (int));
    afuera.write((char*)&al3 , sizeof (Alumno) );
    afuera.write((char*)&index , sizeof (int));
    afuera.write((char*)&al4 , sizeof (Alumno) );
    afuera.write((char*)&index , sizeof (int));
    afuera.write((char*)&al5 , sizeof (Alumno) );
    afuera.write((char*)&index , sizeof (int));



    afuera.close();

    FixedRecord fr(file_name , FREE_LIST);
    fr.load();



    cout<<"DESPUES DE DELETE"<<endl;
    fr.load();

    fr.add(al4);
    fr.add(al4);
    fr.add(al4);
    fr.add(al4);
    fr.add(al4);
    fr.add(al4);
    fr.add(al4);
    fr.add(al4);
    fr.add(al4);
    fr.add(al4);
    fr.add(al4);
    fr.add(al4);
    fr.add(al4);
    fr.add(al4);
    fr.add(al4);
    fr.add(al4);
    fr.add(al4);
    fr.add(al4);
    fr.add(al4);
    fr.add(al4);
    fr.add(al4);
    fr.add(al4);
    fr.add(al4);
    fr.add(al4);
    fr.add(al4);
    fr.add(al4);
    fr.add(al4);
    fr.add(al4);
    fr.add(al4);


        cout<<"DESPUES DE ADD"<<endl;
    fr.load();


    return 0;


}
