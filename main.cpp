#include <iostream>
#include <vector>
#include <string>
using namespace std;
class Evento;
class Conferencia;
class Conferencista;
class Inscripcion;
class Asistente;
class GestorInscripciones;
class ConferenciaMagistral;
class Taller;
template <typename T>

class Gestor {
protected:
    vector<T*> items;

public:
    // Agregar un item a la lista
    void agregarItem(T* item) {
        items.push_back(item);
    }

    // Mostrar todos los items
    void mostrarItems() const {
        if(items.size()==0){
            cout<< "No hay datos para mostrar."<<endl;
        }else
        for (const auto& item : items) {
            item->mostrarDatos();  // Mostrar detalles de cada item
            cout << "-----------------------\n";
        }
    }

    T* seleccionarItem() {
        if( items.size()==0){
            cout<< "No hay datos para mostrar."<<endl;
        }else{
        int id = 0;
        int x = 0;
        for (const auto& item : items) {
            cout << "Identificacion: " << id << ", ";
            item->mostrarDatos();  // Mostrar detalles de cada item
            cout << "-----------------------\n";
            id++;
        }
        cout << "Escriba id del deseado: " << endl;
        cin >> x;
        return items[x];}
    }
    void eliminarItem() {
        int id = 0;
        int x = 0;
        for (const auto& item : items) {
            cout << "Identificacion: " << id << ", ";
            item->mostrarDatos();  // Mostrar detalles de cada item
            cout << "-----------------------\n";
            id++;
        }
        cout << "Escriba id del item que se desea eliminar: " << endl;
        cin >> x;
        items.erase(items.begin()+x);
    }
    void removerItem(T* t){
        int x = 0;
        for (const auto& item : items) {
            if(item == t)
                items.erase(items.begin()+x);
            x++;
        }

    }
    int tamanioDeLista(){
        return items.size();
    }
    void removerLista(){
        int x = 0;
        for (const auto& item : items) {
            items.erase(items.begin()+x);
            delete item;
            x++;
        }

    }
};



// Clase Conferencista
class Conferencista {
    string nombre;
    string especialidad;
    string biografia;
    Gestor<Conferencia> *confparticipadas = new Gestor<Conferencia>;
public:
    Conferencista(string n, string e, string b) : nombre(n), especialidad(e), biografia(b) {}

    void mostrarDatos() const {
        cout << "Conferencista: " << nombre << endl;
    }
    string getNombre(){
        return nombre;
    }
    void agregarConferencia(Conferencia* c){
        confparticipadas->agregarItem(c);
    }
    void eliminarConferencia(Conferencia* c){
        confparticipadas->removerItem(c);
    }
};


class Conferencia {
protected:
    string horario;
    string titulo;
    string descripcion;
    Conferencista* conferencista;

public:
    Conferencia(string t, string d, string h, Conferencista* c)
            : titulo(t), descripcion(d), horario(h), conferencista(c) {
        conferencista->agregarConferencia(this);
    }

    void mostrarDatos() const{
        cout << "Conferencia: " << titulo << ", Descripcion: " << descripcion << ", Horario: " << horario << ", Conferencista: " << conferencista->getNombre()<< endl;
    }
    ~Conferencia(){
        conferencista->eliminarConferencia(this);
    }

};


class Evento {
private:
    string titulo;
    string descripcion;
    string fecha;
    string lugar;
    int maxAsistentes;
    Gestor<Conferencia> *confdelEvento = new Gestor<Conferencia>;
    friend Inscripcion;

public:

    Evento(string t, string d, string f, string l, int m)
            : titulo(t), descripcion(d), fecha(f), lugar(l), maxAsistentes(m) {}

    void mostrarDatos()
    {
        cout << "Evento: " << titulo << ", Descripcion: " << descripcion << ", Fecha: " << fecha << ", Lugar: " << lugar<< endl;
    }
    void agregarConferencia(Conferencia* c){
        confdelEvento->agregarItem(c);
    }


    // Sobrecarga de operadores para comparar eventos por fecha
    bool operator<(const Evento& otro) const {
        return fecha < otro.fecha;
    }
    bool operator>(const Evento& otro) const {
        return fecha > otro.fecha;
    }

    // Sobrecarga de operadores para comparar eventos por número máximo de asistentes
    bool operator<=(const Evento& otro) const {
        return maxAsistentes <= otro.maxAsistentes;
    }
    bool operator>=(const Evento& otro) const {
        return maxAsistentes >= otro.maxAsistentes;
    }
    ~Evento(){
        confdelEvento->removerLista();
    }
};

// Clase contenedora para Inscripciones
class GestorInscripciones : public Gestor<Inscripcion> {

public:
    // Sobrecarga de operadores para comparar el número total de inscripciones
    bool operator<(const GestorInscripciones& otro) const {
        return items.size() < otro.items.size();
    }

    bool operator>(const GestorInscripciones& otro) const {
        return items.size() > otro.items.size();
    }
};

// Clase Asistente
class Asistente {
    string nombre;
    string email;
    GestorInscripciones *eventosInscriptos = new GestorInscripciones;
    friend Inscripcion;
public:
    Asistente(string n, string e) : nombre(n), email(e) {}

    void mostrarDatos() const {
        cout << "Asistente: " << nombre << " - Email: " << email << endl;
    }
    void agregarInscripcion(Inscripcion* e){
        eventosInscriptos->agregarItem(e);
    }
    void eliminarInscripcion(Inscripcion* e){
        eventosInscriptos->removerItem(e);
    }
    ~Asistente(){
    }
};



// Clase Inscripcion
class Inscripcion {
    Evento* evento;
    Gestor<Asistente>* asistInscriptos = new Gestor<Asistente>;

public:
    Inscripcion(Evento* even)
            : evento(even){}

    void mostrarDatos() const {
        cout << "Inscripción:" << endl;
        evento->mostrarDatos();
        cout << "Personas Inscriptas:" << endl;
        asistInscriptos->mostrarItems();
    }
    void agregarAsistentes(Asistente* a){
        if(evento->maxAsistentes>asistInscriptos->tamanioDeLista()) {
            asistInscriptos->agregarItem(a);
            a->agregarInscripcion(this);
            cout << "Asistente añadido con exito" << endl;
        }
        else
            cout << "Asistente no añadido: Evento lleno" << endl;
    }
    void eliminarAsistentes(){
       asistInscriptos->eliminarItem();
    }

    // Sobrecarga de operadores para comparar inscripciones por número de asistentes permitidos
    bool operator<(const Inscripcion& otro) const {
        return evento->maxAsistentes < otro.evento->maxAsistentes;
    }

    bool operator>(const Inscripcion& otro) const {
        return evento->maxAsistentes > otro.evento->maxAsistentes;
    }
};


class ConferenciaMagistral: public Conferencia{
private:
    string tema;
public:
    ConferenciaMagistral(string t, string d, string h, Conferencista* c, string m)
            : Conferencia(t, d, h, c),tema(m){}
            void mostrarDatos() {
                cout << "Conferencia: " << titulo << ", Descripcion: " << descripcion << ", Horario: " << horario << ", Conferencista: " << conferencista->getNombre()<< ", Tema: " << tema << endl;
    }

};
class Taller: public Conferencia{
private:
    string material;
public:
    Taller(string t, string d, string h, Conferencista* c, string m)
            : Conferencia(t, d, h, c),material(m){}
    void mostrarDatos() {
        cout << "Conferencia: " << titulo << ", Descripcion: " << descripcion << ", Horario: " << horario << ", Conferencista: " << conferencista->getNombre()<< ", Material: " << material << endl;
    }

};
class Menu {
private:
    GestorInscripciones* todasInscripciones;
    Gestor<Evento>* todosEventos;
    Gestor<Conferencia>* todasConferencias;
    Gestor<Conferencista>* todosConferencistas;
    Gestor<Asistente>* todosAsistentes;
    char eleccion;
public:
    Menu(GestorInscripciones* i, Gestor<Evento>* e, Gestor<Conferencia>* g, Gestor<Conferencista>* c, Gestor<Asistente>* a) : todasInscripciones(i), todosEventos(e), todasConferencias(g), todosConferencistas(c), todosAsistentes(a){}
    char PantallaPrincipal(){
        do {
            cout << "Creacion de [E]vento, [R]egistro de Conferencias, Registro de [A]sistentes, [C]onsulta de Conferencias, Registro de [I]nscripciones, [T]erminar programa" << endl;
            cin >> eleccion;
            switch (eleccion) {
                case 'E':
                    PantallaEventos();
                    break;
                case 'R':
                    PantallaRegistroConferencias();
                    break;
                case 'C':
                    PantallaConsulta();
                    break;
                case 'I':
                    PantallaInscripciones();
                    break;
                case 'A':
                    PantallaRegistroAsistentes();
                    break;
                case 'T':
                    return 'T';
                default:
                    cout << "Tecla no reconocida" << endl;

            }
        }while(eleccion!='T');
        return 'T';
    };
    void PantallaEventos(){
        string titulo;
        string descripcion;
        string fecha;
        string lugar;
        int maxAsistentes;

        cout << "Titulo del evento: ";
        cin >> titulo;
        cout << "Descripcion del evento: ";
        cin >> descripcion;
        cout << "Fecha del evento: ";
        cin >> fecha;
        cout << "Lugar del evento: ";
        cin >> lugar;
        cout << "Asistencia maxima del evento: ";
        cin >> maxAsistentes;
        Evento* evento = new Evento(titulo, descripcion, fecha, lugar, maxAsistentes);
        todosEventos->agregarItem(evento);
        todasInscripciones->agregarItem(new Inscripcion(evento));
    };
    void PantallaRegistroConferencias(){
        if(todosEventos->tamanioDeLista()==0){
            cout<<"No hay eventos disponibles a los que se le puedan agregar conferencias"<<endl;
            return;
        }
        if(todosConferencistas->tamanioDeLista()==0){
            cout<<"No hay conferencistas disponibles"<<endl;
            return;
        }
        Evento* evento;
        string horario;
        string titulo;
        string descripcion;
        Conferencista* conferencista;
        string material;
        string tema;
        cout << "Seleccione a que evento pertenecera la conferencia: ";
        evento= todosEventos->seleccionarItem();
        cout << "Titulo de la conferencia: ";
        cin >> titulo;
        cout << "Descripcion de la conferencia: ";
        cin >> descripcion;
        cout << "Horario de la conferencia: ";
        cin >> horario;
        cout << "Seleccione el conferencista de la conferencia: " << endl;
        conferencista= todosConferencistas->seleccionarItem();
        cout<< "Es una conferencia [M]agistral o un Ta[L]ler?" << endl;
        cin>>eleccion;
        switch (eleccion) {
            case 'M':
                cout << "Tema de la conferencia magistral: ";
                cin >> tema;
                ConferenciaMagistral* conferenciaMagistral;
                conferenciaMagistral = new ConferenciaMagistral(titulo, descripcion, horario, conferencista, tema);
                todasConferencias->agregarItem(conferenciaMagistral);
                evento->agregarConferencia(conferenciaMagistral);
                break;
            case 'L':
                cout << "Material del taller: ";
                cin >> material;
                Taller* taller;
                taller = new Taller(titulo, descripcion, horario, conferencista, material);
                todasConferencias->agregarItem(taller);
                evento->agregarConferencia(taller);
                break;
            case 'N':
                Conferencia* conferencia;
                conferencia = new Conferencia(titulo, descripcion, horario, conferencista);
                todasConferencias->agregarItem(conferencia);
                evento->agregarConferencia(conferencia);
                break;}

    };
    void PantallaRegistroAsistentes(){
        string nombre;
        string email;
        Asistente* asistente;
        cout << "Nombre del asistente: ";
        cin >> nombre;
        cout << "Email del asistente: ";
        cin >> email;
        asistente = new Asistente(nombre, email);
        todosAsistentes->agregarItem(asistente);

    };
    void PantallaConsulta(){
        todasConferencias->mostrarItems();

    };
    void PantallaInscripciones(){
        if(todasInscripciones->tamanioDeLista()==0){
            cout<<"No hay inscripciones en este momento."<<endl;
            return;
        }
        Asistente* a;
        Inscripcion* i;
        cout<< "[M]ostrar inscripciones, Inscribir [A]sistente, [D]esinscribir asistente" << endl;
        cin>>eleccion;
        switch (eleccion) {
            case 'M':
                todasInscripciones->mostrarItems();
                break;
            case 'A':
                cout << "Seleccione el evento al que se le desea agregar asistentes: "<< endl;
                i = todasInscripciones->seleccionarItem();
                cout << "Seleccione el asistente a agregar: "<< endl;
                a = todosAsistentes->seleccionarItem();
                i->agregarAsistentes(a);
                break;
            case 'D':
                cout << "Seleccione el evento al que se le desea eliminar asistentes: "<< endl;
                todasInscripciones->seleccionarItem()->eliminarAsistentes();
                break;
            default:
                return;

        }


    };

};

int main() {
    auto mainInscripciones = new  GestorInscripciones;
    auto mainEventos = new Gestor<Evento>;
    auto mainConferencia = new Gestor<Conferencia>;
    auto mainConferencista = new Gestor<Conferencista>;
    auto mainAsistentes = new Gestor<Asistente>;
    char sentinela = 0;
    auto ejemplo1Evento =new Evento("Rural", "Exposicion agropecuaria", "30/9/2024","Santa fe 2280" ,20);
    auto ejemplo1Inscripcion= new Inscripcion(ejemplo1Evento);
    auto ejemplo1conferencista= new Conferencista("Rodolfo", "Musico", "Guitarreo toda su vida");
    auto ejemplo1Taller= new Taller("Muscique", "Taller de musica", "19:00", ejemplo1conferencista, "Guitarra");
    auto ejemplo1Asistente=new Asistente("Leon", "leon@mail.com");
    auto ejemplo2Evento =new Evento("Techno", "Exposicion de tecnologia", "11/10/2024","Jorge Newbery 2280" ,15);
    auto ejemplo2Inscripcion= new Inscripcion(ejemplo2Evento);
    auto ejemplo2conferencista= new Conferencista("Otto", "Investigador", "Ojo por ojo controlo todo");
    auto ejemplo2ConferenciaMagistral= new ConferenciaMagistral("Tecnoclan", "Charla de tecnologia", "9:00", ejemplo2conferencista, "Tecnologia");
    auto ejemplo2Asistente=new Asistente("Carlos", "carlos@mail.net");
    mainEventos->agregarItem(ejemplo1Evento);
    mainEventos->agregarItem(ejemplo2Evento);
    mainInscripciones->agregarItem(ejemplo1Inscripcion);
    mainInscripciones->agregarItem(ejemplo2Inscripcion);
    mainConferencista->agregarItem(ejemplo1conferencista);
    mainConferencista->agregarItem(ejemplo2conferencista);
    mainConferencia->agregarItem(ejemplo1Taller);
    mainConferencia->agregarItem(ejemplo2ConferenciaMagistral);
    mainAsistentes->agregarItem(ejemplo1Asistente);
    mainAsistentes->agregarItem(ejemplo2Asistente);


    auto mainMenu = new Menu(mainInscripciones, mainEventos, mainConferencia, mainConferencista, mainAsistentes);
    do{
       sentinela = mainMenu->PantallaPrincipal();
    }while(sentinela != 'T');
    return 0;
}
