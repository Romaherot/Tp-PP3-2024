#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Evento {
protected:
    string titulo;
    string descripcion;
    string fecha;
    string lugar;

public:
    int maxAsistentes;
    Evento(string t, string d, string f, string l, int m)
            : titulo(t), descripcion(d), fecha(f), lugar(l), maxAsistentes(m) {}

    virtual void mostrarEvento() const = 0;  // Método virtual puro
    virtual ~Evento() {}  // Destructor virtual

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

// Clase derivada Conferencia
class Conferencia : public Evento {
    string horario;

public:
    Conferencia(string t, string d, string f, string l, int m, string h)
            : Evento(t, d, f, l, m), horario(h) {}

    void mostrarEvento() const override {
        cout << "Conferencia: " << titulo << "\nDescripción: " << descripcion
             << "\nFecha: " << fecha << "\nLugar: " << lugar
             << "\nMáx. Asistentes: " << maxAsistentes << "\nHorario: " << horario << endl;
    }
};

// Clase contenedora para Conferencias
class GestorConferencias {
    vector<Conferencia> conferencias;

public:
    // Agregar una conferencia a la lista
    void agregarConferencia(Conferencia conf) {
        conferencias.push_back(conf);
    }

    // Mostrar todas las conferencias
    void mostrarConferencias() const {
        for (const auto& conf : conferencias) {
            conf.mostrarEvento();  // Mostrar detalles de cada conferencia
            cout << "-----------------------\n";
        }
    }
};

// Clase Asistente
class Asistente {
    string nombre;
    string email;

public:
    Asistente(string n, string e) : nombre(n), email(e) {}

    void mostrarAsistente() const {
        cout << "Asistente: " << nombre << " - Email: " << email << endl;
    }
};

// Clase Inscripcion
class Inscripcion {
    Conferencia* conferencia;
    Asistente* asistente;

public:
    Inscripcion(Conferencia* conf, Asistente* asis)
            : conferencia(conf), asistente(asis) {}

    void mostrarInscripcion() const {
        cout << "Inscripción:" << endl;
        conferencia->mostrarEvento();
        asistente->mostrarAsistente();
        cout << "-----------------------\n";
    }

    // Sobrecarga de operadores para comparar inscripciones por número de asistentes permitidos
    bool operator<(const Inscripcion& otro) const {
        return conferencia->maxAsistentes < otro.conferencia->maxAsistentes;
    }

    bool operator>(const Inscripcion& otro) const {
        return conferencia->maxAsistentes > otro.conferencia->maxAsistentes;
    }
};

// Clase contenedora para Inscripciones
class GestorInscripciones {
    vector<Inscripcion> inscripciones;

public:
    // Agregar una inscripción
    void agregarInscripcion(Inscripcion inscripcion) {
        inscripciones.push_back(inscripcion);
    }

    // Mostrar todas las inscripciones
    void mostrarInscripciones() const {
        for (const auto& inscripcion : inscripciones) {
            inscripcion.mostrarInscripcion();  // Mostrar detalles de cada inscripción
        }
    }

    // Sobrecarga de operadores para comparar el número total de inscripciones
    bool operator<(const GestorInscripciones& otro) const {
        return inscripciones.size() < otro.inscripciones.size();
    }

    bool operator>(const GestorInscripciones& otro) const {
        return inscripciones.size() > otro.inscripciones.size();
    }
};

int main() {
    // Crear conferencia (derivada de Evento)
    Conferencia conf1("AI Conference", "Conferencia sobre IA", "10/10/2024", "Auditorio A", 100, "10:00 AM");
    Conferencia conf2("Robotics Conference", "Conferencia sobre robótica", "11/10/2024", "Auditorio B", 150, "11:00 AM");

    // Crear gestor de conferencias
    GestorConferencias gestorConferencias;
    gestorConferencias.agregarConferencia(conf1);
    gestorConferencias.agregarConferencia(conf2);

    // Mostrar todas las conferencias
    gestorConferencias.mostrarConferencias();

    // Crear asistente e inscripción
    Asistente asistente1("Jane Smith", "jane@example.com");
    Asistente asistente2("Carlos Ruiz", "carlos@example.com");

    // Crear gestor de inscripciones
    GestorInscripciones gestorInscripciones;

    // Registrar inscripciones
    Inscripcion inscripcion1(&conf1, &asistente1);
    Inscripcion inscripcion2(&conf2, &asistente2);
    gestorInscripciones.agregarInscripcion(inscripcion1);
    gestorInscripciones.agregarInscripcion(inscripcion2);

    // Mostrar todas las inscripciones
    gestorInscripciones.mostrarInscripciones();

    // Comparaciones con sobrecarga de operadores
    cout << "Comparando conferencias por fecha: " << (conf1 < conf2 ? "Conf1 es antes" : "Conf2 es antes") << endl;
    cout << "Comparando inscripciones por asistentes: " << (inscripcion1 < inscripcion2 ? "Conf1 tiene menos asistentes" : "Conf2 tiene menos asistentes") << endl;
    cout << "Comparando gestores por número de inscripciones: " << (gestorInscripciones > gestorInscripciones ? "Gestor 1 tiene más inscripciones" : "Gestor 2 tiene más inscripciones") << endl;

    return 0;
}
