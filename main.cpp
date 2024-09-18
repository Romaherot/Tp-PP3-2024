#include <iostream>
#include <vector>
#include <string>
using namespace std;
class Evento;
class Conferencia;
class Conferencista;
class Inscripcion;
class Asistente;
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
        for (const auto& item : items) {
            item->mostrarDatos();  // Mostrar detalles de cada item
            cout << "-----------------------\n";
        }
    }

    T* seleccionarItem() {
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
        return items[x];
    }
    int tamanioDeLista(){
        return items.size();
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
};


class Conferencia {
private:
    string horario;
    string titulo;
    string descripcion;
    Conferencista* conferencista;

public:
    Conferencia(string t, string d, string h, Conferencista* c)
            : titulo(t), descripcion(d), horario(h), conferencista(c) {}

    void mostrarDatos() const{
        cout << "Conferencia: " << titulo << ", Descripcion: " << descripcion << ", Horario: " << horario << ", Conferencista: " << conferencista->getNombre()<< endl;
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
        cout << "Conferencia: " << titulo << ", Descripcion: " << descripcion << ", Fecha: " << fecha << ", Lugar: " << lugar<< endl;
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
};

// Clase Asistente
class Asistente {
    string nombre;
    string email;
    Gestor<Evento> *eventosInscriptos = new Gestor<Evento>;
    friend Inscripcion;
public:
    Asistente(string n, string e) : nombre(n), email(e) {}

    void mostrarDatos() const {
        cout << "Asistente: " << nombre << " - Email: " << email << endl;
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
        asistInscriptos->mostrarItems();
        cout << "-----------------------\n";
    }
    void agregarAsistentes(Asistente* a){
        if(evento->maxAsistentes>asistInscriptos->tamanioDeLista()) {
            asistInscriptos->agregarItem(a);
            a->eventosInscriptos->agregarItem(evento);
            cout << "Asistente añadido con exito" << endl;
        }
        else
            cout << "Asistente no añadido: Evento lleno" << endl;
    }

    // Sobrecarga de operadores para comparar inscripciones por número de asistentes permitidos
    bool operator<(const Inscripcion& otro) const {
        return evento->maxAsistentes < otro.evento->maxAsistentes;
    }

    bool operator>(const Inscripcion& otro) const {
        return evento->maxAsistentes > otro.evento->maxAsistentes;
    }
};

// Clase contenedora para Inscripciones
class GestorInscripciones : Gestor<Inscripcion> {

public:
    // Sobrecarga de operadores para comparar el número total de inscripciones
    bool operator<(const GestorInscripciones& otro) const {
        return items.size() < otro.items.size();
    }

    bool operator>(const GestorInscripciones& otro) const {
        return items.size() > otro.items.size();
    }
};
class Menu {
private:
    GestorInscripciones* todasInscripciones;
    Gestor<Evento>* todosEventos;
    Gestor<Conferencia>* todasConferencias;
    Gestor<Conferencista>* todosConferencistas;
    char eleccion;
public:
    Menu(GestorInscripciones* i, Gestor<Evento>* e, Gestor<Conferencia>* g) : todasInscripciones(i), todosEventos(e), todasConferencias(g) {}
    void PantallaPrincipal(){
        do {
            cout << "Creacion de [E]vento, [R]egistro de Conferencias, [C]onsulta de Conferencias, Registro de [I]nscripciones, [T]erminar programa" << endl;
            cin >> eleccion;
            switch (eleccion) {
                case 'E':
                    PantallaEventos();
                    break;
                case 'R':
                    PantallaRegistro();
                    break;
                case 'C':
                    PantallaConsulta();
                    break;
                case 'I':
                    PantallaInscripciones();
                    break;
                case 'T':
                    return;
                default:
                    cout << "Tecla no reconocida" << endl;

            }
        }while(eleccion!='T');
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
        todosEventos->agregarItem(new Evento(titulo, descripcion, fecha, lugar, maxAsistentes));
    };
    void PantallaRegistro(){
        Evento* evento;
        string horario;
        string titulo;
        string descripcion;
        Conferencista* conferencista;
        Conferencia* conferencia;
        cout << "Seleccione a que evento pertenecera la conferencia: ";
        evento= todosEventos->seleccionarItem();
        cout << "Titulo de la conferencia: ";
        cin >> titulo;
        cout << "Descripcion de la conferencia: ";
        cin >> descripcion;
        cout << "Horario de la conferencia: ";
        cin >> horario;
        cout << "Seleccione el conferencista de la conferencia: ";
        conferencista= todosConferencistas->seleccionarItem();
        conferencia = new Conferencia(titulo, descripcion, horario, conferencista);
        todasConferencias->agregarItem(conferencia);
        evento->agregarConferencia(conferencia);

    };
    void PantallaConsulta(){
        todasConferencias->mostrarItems();

    };
    void PantallaInscripciones(){
        Evento* evento;

    };

};

int main() {

    return 0;
}
