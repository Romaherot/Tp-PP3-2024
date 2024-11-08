#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Clase base para Gestor genérico
template <typename T>
class Gestor {
protected:
    vector<T*> items;

public:
    void agregarItem(T* item) {
        items.push_back(item);
    }

    void mostrarItems() const {
        if(items.empty()){
            cout << "No hay datos para mostrar." << endl;
        } else {
            for (const auto& item : items) {
                item->mostrarDatos();
                cout << "-----------------------\n";
            }
        }
    }

    T* seleccionarItem() {
        if(items.empty()){
            cout << "No hay datos para mostrar." << endl;
            return nullptr;
        } else {
            int id = 0;
            int x = 0;
            for (const auto& item : items) {
                cout << "Id: " << id << ", ";
                item->mostrarDatos();
                cout << "-----------------------\n";
                id++;
            }
            cout << "Escriba id del deseado: " << endl;
            cin >> x;
            if(x >= 0 && x < items.size()) {
                return items[x];
            } else {
                cout << "ID inválido." << endl;
                return nullptr;
            }
        }
    }

    void eliminarItem() {
        if(items.empty()){
            cout << "No hay elementos para eliminar." << endl;
            return;
        }
        int id = 0;
        int x = 0;
        for (const auto& item : items) {
            cout << "Id: " << id << ", ";
            item->mostrarDatos();
            cout << "-----------------------\n";
            id++;
        }
        cout << "Escriba id del item que se desea eliminar: " << endl;
        cin >> x;
        if(x >= 0 && x < items.size()) {
            delete items[x];
            items.erase(items.begin() + x);
            cout << "Elemento eliminado con éxito." << endl;
        } else {
            cout << "ID inválido." << endl;
        }
    }

    void removerItem(T* t){
        for (auto it = items.begin(); it != items.end(); ++it) {
            if(*it == t) {
                delete *it;
                items.erase(it);
                return;
            }
        }
    }

    int tamanioDeLista(){
        return items.size();
    }

    void removerLista(){
        for (auto& item : items) {
            delete item;
        }
        items.clear();
    }

    ~Gestor() {
        removerLista();
    }
};

// Clase Conferencista
class Conferencista {
    string nombre;
    string especialidad;
    string biografia;
    Gestor<class Conferencia> *confparticipadas = new Gestor<class Conferencia>;
public:
    Conferencista(string n, string e, string b) : nombre(n), especialidad(e), biografia(b) {}

    void mostrarDatos() const {
        cout << "Conferencista: " << nombre << ", Especialidad: " << especialidad << endl;
    }
    string getNombre() const {
        return nombre;
    }
    void agregarConferencia(Conferencia* c){
        confparticipadas->agregarItem(c);
    }
    void eliminarConferencia(Conferencia* c){
        confparticipadas->removerItem(c);
    }
    // Overloaded operators for comparing speakers by number of conferences
    bool operator<(const Conferencista& otro) const {
        return confparticipadas->tamanioDeLista() < otro.confparticipadas->tamanioDeLista();
    }

    bool operator>(const Conferencista& otro) const {
        return confparticipadas->tamanioDeLista() > otro.confparticipadas->tamanioDeLista();
    }

    bool operator==(const Conferencista& otro) const {
        return confparticipadas->tamanioDeLista() == otro.confparticipadas->tamanioDeLista();
    }

    int getNumConferencias() const {
        return confparticipadas->tamanioDeLista();
    }

    ~Conferencista() {
        delete confparticipadas;
    }
};

class Inscripcion;

// Clase Evento
class Evento {
private:
    string titulo;
    string descripcion;
    string fecha;
    string lugar;
    int maxAsistentes;
    Gestor<Conferencia>* confDelEvento = new Gestor<Conferencia>;
    friend Inscripcion;

public:
    Evento(string t, string d, string f, string l, int m)
            : titulo(t), descripcion(d), fecha(f), lugar(l), maxAsistentes(m) {}

    virtual void mostrarDatos() const {
        cout << "Evento: " << titulo << ", Descripción: " << descripcion
             << ", Fecha: " << fecha << ", Lugar: " << lugar
             << ", Máximo de asistentes: " << maxAsistentes << endl;
    }
    void agregarConferencia(Conferencia* c){
        confDelEvento->agregarItem(c);
    }
    void eliminarConferencia(Conferencia* c){
        confDelEvento->removerItem(c);
    }
    // Overloaded operators for comparing events by date
    bool operator<(const Evento& otro) const {
        return fecha < otro.fecha;
    }

    bool operator>(const Evento& otro) const {
        return fecha > otro.fecha;
    }

    bool operator==(const Evento& otro) const {
        return fecha == otro.fecha;
    }

    string getFecha() const {
        return fecha;
    }
    virtual ~Evento() {}
};

// Clase Conferencia
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

    virtual void mostrarDatos() const {
        cout << "Conferencia: " << titulo << ", Descripción: " << descripcion
             << ", Horario: " << horario << ", Conferencista: " << conferencista->getNombre() << endl;
    }

    virtual ~Conferencia() {
        conferencista->eliminarConferencia(this);
    }
};

// Clase ConferenciaMagistral
class ConferenciaMagistral : public Conferencia {
private:
    string tema;
public:
    ConferenciaMagistral(string t, string d, string h, Conferencista* c, string tm)
            : Conferencia(t, d, h, c), tema(tm) {}

    void mostrarDatos() const override {
        cout << "Conferencia Magistral: " << titulo << ", Descripción: " << descripcion
             << ", Horario: " << horario << ", Conferencista: " << conferencista->getNombre()
             << ", Tema: " << tema << endl;
    }
};

// Clase Taller
class Taller : public Conferencia {
private:
    string material;
public:
    Taller(string t, string d, string h, Conferencista* c, string m)
            : Conferencia(t, d, h, c), material(m) {}

    void mostrarDatos() const override {
        cout << "Taller: " << titulo << ", Descripción: " << descripcion
             << ", Horario: " << horario << ", Conferencista: " << conferencista->getNombre()
             << ", Material: " << material << endl;
    }
};

// Clase Asistente
class Asistente {
    string nombre;
    string email;
    string telefono;
    Gestor<Inscripcion> *eventosInscriptos = new Gestor<Inscripcion>;
    friend class Inscripcion;
public:
    Asistente(string n, string e, string t) : nombre(n), email(e), telefono(t) {}

    void mostrarDatos() const {
        cout << "Asistente: " << nombre << " - Email: " << email << " - Teléfono: " << telefono << endl;
    }
    void agregarInscripcion(Inscripcion* e){
        eventosInscriptos->agregarItem(e);
    }
    void eliminarInscripcion(Inscripcion* e){
        eventosInscriptos->removerItem(e);
    }
    // Overloaded operators for comparing attendees by number of registered events
    bool operator<(const Asistente& otro) const {
        return eventosInscriptos->tamanioDeLista() < otro.eventosInscriptos->tamanioDeLista();
    }

    bool operator>(const Asistente& otro) const {
        return eventosInscriptos->tamanioDeLista() > otro.eventosInscriptos->tamanioDeLista();
    }

    bool operator==(const Asistente& otro) const {
        return eventosInscriptos->tamanioDeLista() == otro.eventosInscriptos->tamanioDeLista();
    }

    int getNumInscripciones() const {
        return eventosInscriptos->tamanioDeLista();
    }

    string getNombre() const {
        return nombre;
    }
    ~Asistente(){
        delete eventosInscriptos;
    }
};


// Clase Inscripcion
class Inscripcion {
    Evento* evento;
    Gestor<Asistente>* asistInscriptos = new Gestor<Asistente>;

public:
    Inscripcion(Evento* even) : evento(even) {}

    void mostrarDatos() const {
        cout << "Inscripción:" << endl;
        evento->mostrarDatos();
        cout << "Personas Inscriptas:" << endl;
        asistInscriptos->mostrarItems();
    }
    void agregarAsistentes(Asistente* a){
    if(evento->maxAsistentes>asistInscriptos->tamanioDeLista()) {
        asistInscriptos->agregarItem(a);
        a->eventosInscriptos->agregarItem(this);
        cout << "Asistente añadido con exito" << endl;
    }
    else
    cout << "Asistente no añadido: Evento lleno" << endl;
}
    void eliminarAsistentes(){
        asistInscriptos->eliminarItem();
    }

    ~Inscripcion() {
        delete asistInscriptos;
    }
};


class Menu {
private:
    Gestor<Inscripcion>* todasInscripciones;
    Gestor<Evento>* todosEventos;
    Gestor<Conferencia>* todasConferencias;
    Gestor<Conferencista>* todosConferencistas;
    Gestor<Asistente>* todosAsistentes;
    char eleccion;
public:
    Menu(Gestor<Inscripcion>* i, Gestor<Evento>* e, Gestor<Conferencia>* g, Gestor<Conferencista>* c, Gestor<Asistente>* a)
            : todasInscripciones(i), todosEventos(e), todasConferencias(g), todosConferencistas(c), todosAsistentes(a) {}

    char PantallaPrincipal(){
        do {
            cout <<  "\033[1m|=================================|\033[0m" << endl;
            cout <<  "\033[1m1)\033[0mCreacion de Evento\n"
                    "\033[1m2)\033[0mRegistro de Conferencias\n"
                    "\033[1m3)\033[0mRegistro de Asistentes\n"
                    "\033[1m4)\033[0mConsulta de Conferencias\n"
                    "\033[1m5)\033[0mRegistro de Inscripciones\n"
                    "\033[1m6)\033[0mCrear Conferencista\n"
                    "\033[1m7)\033[0mOperaciones de comparación\n"
                    "\033[1m8)\033[0mTerminar programa" << endl;
            cout <<  "\033[1m|---------------------------------|\033[0m" << endl;
            cin >> eleccion;
            switch (eleccion) {
                case '1':
                    PantallaEventos();
                    break;
                case '2':
                    PantallaRegistroConferencias();
                    break;
                case '3':
                    PantallaRegistroAsistentes();
                    break;
                case '4':
                    PantallaConsulta();
                    break;
                case '5':
                    PantallaInscripciones();
                    break;
                case '6':
                    PantallaCrearConferencista();
                    break;
                case '7':
                    PantallaOperacionesComparacion();
                    break;
                case '8':
                    return 'T';
                default:
                    system("CLS");
                    cout << "Tecla no reconocida" << endl;
            }
            cout << "Presione enter para continuar." << endl;
            cin.clear();
            cin.get();
            cin.get();
            system("CLS");
        } while(eleccion != 'T');
        return 'T';
    }

    void PantallaEventos(){
        string titulo, descripcion, fecha, lugar;
        int maxAsistentes;

        cout << "Titulo del evento: ";
        cin.ignore();
        getline(cin, titulo);
        cout << "Descripcion del evento: ";
        getline(cin, descripcion);
        cout << "Fecha del evento: ";
        getline(cin, fecha);
        cout << "Lugar del evento: ";
        getline(cin, lugar);
        cout << "Asistencia maxima del evento: ";
        cin >> maxAsistentes;

        Evento* evento = new Evento(titulo, descripcion, fecha, lugar, maxAsistentes);
        todosEventos->agregarItem(evento);
        todasInscripciones->agregarItem(new Inscripcion(evento));
        cout << "Evento creado con éxito." << endl;
    }

    void PantallaRegistroConferencias(){
        if(todosEventos->tamanioDeLista() == 0){
            cout << "No hay eventos disponibles. Por favor, cree un evento primero." << endl;
            return;
        }
        if(todosConferencistas->tamanioDeLista() == 0){
            cout << "No hay conferencistas disponibles. Por favor, cree un conferencista primero." << endl;
            return;
        }
        string horario, titulo, descripcion;
        Conferencista* conferencista;
        string material;
        string tema;
        Evento* evento;
        cout << "Seleccione el evento al que pertenece la conferencia: " << endl;
        evento = todosEventos->seleccionarItem();
        if (evento == nullptr) return;

        cout << "Titulo de la conferencia: ";
        cin.ignore();
        getline(cin, titulo);
        cout << "Descripcion de la conferencia: ";
        getline(cin, descripcion);
        cout << "Horario de la conferencia: ";
        getline(cin, horario);
        cout << "Seleccione el conferencista de la conferencia: " << endl;
        conferencista = todosConferencistas->seleccionarItem();
        if (conferencista == nullptr) return;

        cout << "Es una conferencia [M]agistral, un Ta[L]ler o una conferencia [N]ormal?" << endl;
        cin >> eleccion;

        Conferencia* conferencia;
        switch (eleccion) {
            case 'M':
                cout << "Tema de la conferencia magistral: ";
                cin.ignore();
                getline(cin, tema);
                conferencia = new ConferenciaMagistral(titulo, descripcion, horario, conferencista, tema);
                break;
            case 'L':
                cout << "Material del taller: ";
                cin.ignore();
                getline(cin, material);
                conferencia = new Taller(titulo, descripcion, horario, conferencista, material);
                break;
            default:
                conferencia = new Conferencia(titulo, descripcion, horario, conferencista);
                break;
        }

        todasConferencias->agregarItem(conferencia);
        evento->agregarConferencia(conferencia);
        cout << "Conferencia registrada con éxito." << endl;

    }

    void PantallaRegistroAsistentes(){
        string nombre, email, telefono;
        cout << "Nombre del asistente: ";
        cin.ignore();
        getline(cin, nombre);
        cout << "Email del asistente: ";
        getline(cin, email);
        cout << "Teléfono del asistente: ";
        getline(cin, telefono);
        Asistente* asistente = new Asistente(nombre, email, telefono);
        todosAsistentes->agregarItem(asistente);
        cout << "Asistente registrado con éxito." << endl;
    }

    void PantallaConsulta(){
        todasConferencias->mostrarItems();
    }

    void PantallaInscripciones(){
        if(todasInscripciones->tamanioDeLista() == 0){
            cout << "No hay inscripciones en este momento." << endl;
            return;
        }
        Asistente* a;
        Inscripcion* i;
        cout << "[M]ostrar inscripciones, Inscribir [A]sistente, [D]esinscribir asistente" << endl;
        cin >> eleccion;
        switch (eleccion) {
            case 'M':
                todasInscripciones->mostrarItems();
                break;
            case 'A':
                cout << "Seleccione el evento al que se le desea agregar asistentes: " << endl;
                i = todasInscripciones->seleccionarItem();
                if (i == nullptr) return;
                cout << "Seleccione el asistente a agregar: " << endl;
                a = todosAsistentes->seleccionarItem();
                if (a == nullptr) return;
                i->agregarAsistentes(a);
                a->agregarInscripcion(i);
                break;
            case 'D':
                cout << "Seleccione el evento al que se le desea eliminar asistentes: " << endl;
                i = todasInscripciones->seleccionarItem();
                if (i == nullptr) return;
                i->eliminarAsistentes();
                break;
            default:
                cout << "Opción no válida." << endl;
                return;
        }
    }

    void PantallaCrearConferencista() {
        string nombre, especialidad, biografia;

        cout << "Nombre del conferencista: ";
        cin.ignore();
        getline(cin, nombre);

        cout << "Especialidad del conferencista: ";
        getline(cin, especialidad);

        cout << "Biografía del conferencista: ";
        getline(cin, biografia);

        Conferencista* conferencista = new Conferencista(nombre, especialidad, biografia);
        todosConferencistas->agregarItem(conferencista);
        cout << "Conferencista creado con éxito." << endl;
    }
    void PantallaOperacionesComparacion() {
        cout <<  "\033[1m|---------------------------------|\033[0m" << endl;
        cout << "Seleccione la operación de comparación:" << endl;
        cout <<  "\033[1m1)\033[0mComparar eventos por fecha\n"
                 "\033[1m2)\033[0mComparar asistentes por número de inscripciones\n"
                 "\033[1m3)\033[0mComparar conferencistas por número de conferencias"<< endl;
        cout <<  "\033[1m|---------------------------------|\033[0m" << endl;
        int opcion;
        cin >> opcion;

        switch(opcion) {
            case 1:
                CompararEventos();
                break;
            case 2:
                CompararAsistentes();
                break;
            case 3:
                CompararConferencistas();
                break;
            default:
                cout << "Opción no válida." << endl;
        }
    }

    void CompararEventos() {
        if (todosEventos->tamanioDeLista() < 2) {
            cout << "Se necesitan al menos dos eventos para comparar." << endl;
            return;
        }

        cout << "Seleccione el primer evento:" << endl;
        Evento* evento1 = todosEventos->seleccionarItem();
        if (evento1 == nullptr) return;

        cout << "Seleccione el segundo evento:" << endl;
        Evento* evento2 = todosEventos->seleccionarItem();
        if (evento2 == nullptr) return;

        if (*evento1 < *evento2) {
            cout << "El primer evento es anterior al segundo." << endl;
        } else if (*evento1 > *evento2) {
            cout << "El primer evento es posterior al segundo." << endl;
        } else {
            cout << "Ambos eventos son en la misma fecha." << endl;
        }
    }

    void CompararAsistentes() {
        if (todosAsistentes->tamanioDeLista() < 2) {
            cout << "Se necesitan al menos dos asistentes para comparar." << endl;
            return;
        }

        cout << "Seleccione el primer asistente:" << endl;
        Asistente* asistente1 = todosAsistentes->seleccionarItem();
        if (asistente1 == nullptr) return;

        cout << "Seleccione el segundo asistente:" << endl;
        Asistente* asistente2 = todosAsistentes->seleccionarItem();
        if (asistente2 == nullptr) return;

        if (*asistente1 < *asistente2) {
            cout << asistente1->getNombre() << " tiene menos inscripciones que " << asistente2->getNombre() << "." << endl;
        } else if (*asistente1 > *asistente2) {
            cout << asistente1->getNombre() << " tiene más inscripciones que " << asistente2->getNombre() << "." << endl;
        } else {
            cout << "Ambos asistentes tienen el mismo número de inscripciones." << endl;
        }
    }

    void CompararConferencistas() {
        if (todosConferencistas->tamanioDeLista() < 2) {
            cout << "Se necesitan al menos dos conferencistas para comparar." << endl;
            return;
        }

        cout << "Seleccione el primer conferencista:" << endl;
        Conferencista* conferencista1 = todosConferencistas->seleccionarItem();
        if (conferencista1 == nullptr) return;

        cout << "Seleccione el segundo conferencista:" << endl;
        Conferencista* conferencista2 = todosConferencistas->seleccionarItem();
        if (conferencista2 == nullptr) return;

        if (*conferencista1 < *conferencista2) {
            cout << conferencista1->getNombre() << " tiene menos conferencias que " << conferencista2->getNombre() << "." << endl;
        } else if (*conferencista1 > *conferencista2) {
            cout << conferencista1->getNombre() << " tiene más conferencias que " << conferencista2->getNombre() << "." << endl;
        } else {
            cout << "Ambos conferencistas tienen el mismo número de conferencias." << endl;
        }
    }
};

int main() {
    auto mainInscripciones = new Gestor<Inscripcion>;
    auto mainEventos = new Gestor<Evento>;
    auto mainConferencia = new Gestor<Conferencia>;
    auto mainConferencista = new Gestor<Conferencista>;
    auto mainAsistentes = new Gestor<Asistente>;
    char centinela = 0;

    auto mainMenu = new Menu(mainInscripciones, mainEventos, mainConferencia, mainConferencista, mainAsistentes);
    do {
        centinela = mainMenu->PantallaPrincipal();
    } while(centinela != 'T');


    return 0;

}
