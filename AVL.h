#include <iostream>
#include <queue>
#include <vector>
#include <variant>
#include <string>
#include <algorithm>
#include <sstream>
#include <stdexcept>
using namespace std;
 
// 
// Tipo Fecha simple (YYYY-MM-DD)
struct Fecha {
    int anio, mes, dia;
 
    Fecha(int a = 0, int m = 0, int d = 0) : anio(a), mes(m), dia(d) {}
 
    // Parsea "YYYY-MM-DD"
    static Fecha parse(const string& s) {
        // acepta YYYY-MM-DD o DD/MM/YYYY
        Fecha f;
        if (s.size() == 10 && s[4] == '-') {
            f.anio = stoi(s.substr(0, 4));
            f.mes  = stoi(s.substr(5, 2));
            f.dia  = stoi(s.substr(8, 2));
        } else if (s.size() == 10 && s[2] == '/') {
            f.dia  = stoi(s.substr(0, 2));
            f.mes  = stoi(s.substr(3, 2));
            f.anio = stoi(s.substr(6, 4));
        } else {
            throw invalid_argument("Formato de fecha inválido: " + s);
        }
        return f;
    }
 
    string str() const {
        char buf[11];
        snprintf(buf, sizeof(buf), "%04d-%02d-%02d", anio, mes, dia);
        return buf;
    }
 
    bool operator<(const Fecha& o) const {
        if (anio != o.anio) return anio < o.anio;
        if (mes  != o.mes)  return mes  < o.mes;
        return dia < o.dia;
    }
    bool operator>(const Fecha& o) const { return o < *this; }
    bool operator==(const Fecha& o) const {
        return anio == o.anio && mes == o.mes && dia == o.dia;
    }
    bool operator!=(const Fecha& o) const { return !(*this == o); }
    bool operator<=(const Fecha& o) const { return !(o < *this); }
    bool operator>=(const Fecha& o) const { return !(*this < o); }
};
 
// Dirección de fila en el CSV
struct Direccion {
    int sector;
    int offset;
};
 
// Nodo genérico del AVL
template<typename T>
struct CNode {
    T v;
    int alt;
    int des;  // desbalance = alt(der) - alt(izq)
    vector<Direccion> direcciones;
    CNode* nodes[2];
 
    CNode(const T& _v) : v(_v), alt(1), des(0) {
        nodes[0] = nodes[1] = nullptr;
    }
};
 // Árbol AVL genérico

template<typename T>
class CBinTree {
public:
    CBinTree() : root(nullptr), brep(false) {}
    ~CBinTree() { destruir(root); }
 
    // Retorna true si x ya existía (agrega dirección), false si es nuevo
    bool ins(const T& x, Direccion dir);
    bool rem(const T& x);
    bool find(const T& x, CNode<T>**& p);
    void print() const;
    void print_levels() const;
 
    CNode<T>* root;
 
private:
    bool brep;
 
    void destruir(CNode<T>* n);
    int  calcular_altura(CNode<T>* n);
    void verificador(CNode<T>* n);
    void casos(CNode<T>**& m);
    CNode<T>** rep(CNode<T>** p);
    void inorder(CNode<T>* n) const;
    int  height2(CNode<T>* n) const;
    void levels(CNode<T>* n) const;
 
    // Comparación: retorna -1, 0, 1
    static int cmp(const T& a, const T& b) {
        if (a < b) return -1;
        if (a > b) return  1;
        return 0;
    }
};
 
// ── Destructor ─────────────────────
template<typename T>
void CBinTree<T>::destruir(CNode<T>* n) {
    if (!n) return;
    destruir(n->nodes[0]);
    destruir(n->nodes[1]);
    delete n;
}
 
// ── find ───────────────────────────
template<typename T>
bool CBinTree<T>::find(const T& x, CNode<T>**& p) {
    for (p = &root; *p && (*p)->v != x; ) {
        int c = cmp(x, (*p)->v);
        p = &((*p)->nodes[c > 0 ? 1 : 0]);
    }
    return *p != nullptr;
}
 
// ── Altura y desbalance ────────────────────
template<typename T>
int CBinTree<T>::calcular_altura(CNode<T>* n) {
    if (!n) return 0;
    int altizq = calcular_altura(n->nodes[0]);
    int altder = calcular_altura(n->nodes[1]);
    n->alt = 1 + max(altizq, altder);
    n->des = altder - altizq;
    return n->alt;
}
 
// ── Rotaciones / casos AVL ────────────────
template<typename T>
void CBinTree<T>::casos(CNode<T>**& m) {
    // Desbalance derecho (+2)
    if ((*m)->des == 2) {
        CNode<T>* o = (*m)->nodes[1];
        if (o && o->des >= 0) {
            CNode<T>* tmp = *m;
            *m = o;
            tmp->nodes[1] = o->nodes[0];
            o->nodes[0] = tmp;
            calcular_altura(root);
        }
        else if (o && o->des == -1) {
            CNode<T>* p = o->nodes[0];
            if (!p) return;
            CNode<T>* tmp = *m;
            *m = p;
            tmp->nodes[1] = p->nodes[0];
            o->nodes[0]   = p->nodes[1];
            p->nodes[0]   = tmp;
            p->nodes[1]   = o;
            calcular_altura(root);
        }
    }
    if ((*m)->des == -2) {
        CNode<T>* o = (*m)->nodes[0];
        if (o && o->des <= 0) {
            CNode<T>* tmp = *m;
            *m = o;
            tmp->nodes[0] = o->nodes[1];
            o->nodes[1] = tmp;
            calcular_altura(root);
        }
        else if (o && o->des == 1) {
            CNode<T>* p = o->nodes[1];
            if (!p) return;
            CNode<T>* tmp = *m;
            *m = p;
            tmp->nodes[0] = p->nodes[1];
            o->nodes[1]   = p->nodes[0];
            p->nodes[0]   = o;
            p->nodes[1]   = tmp;
            calcular_altura(root);
        }
    }
}
 
template<typename T>
void CBinTree<T>::verificador(CNode<T>* /*n*/) {
    queue<CNode<T>**> q;
    q.push(&root);
    while (!q.empty()) {
        CNode<T>** x = q.front(); q.pop();
        if (!*x) continue;
        if ((*x)->des == 2 || (*x)->des == -2)
            casos(x);
        q.push(&(*x)->nodes[0]);
        q.push(&(*x)->nodes[1]);
    }
}
 
// ── ins ─────────────────────────────────────
template<typename T>
bool CBinTree<T>::ins(const T& x, Direccion dir) {
    CNode<T>** p;
    if (find(x, p)) {
        (*p)->direcciones.push_back(dir);
        return true;  // ya existía
    }
    *p = new CNode<T>(x);
    (*p)->direcciones.push_back(dir);
    calcular_altura(root);
    verificador(root);
    return false;  // recién insertado
}
 
// ── rem ─────────────────────────────────────
template<typename T>
bool CBinTree<T>::rem(const T& x) {
    CNode<T>** p;
    if (!find(x, p)) return false;
 
    if ((*p)->nodes[0] && (*p)->nodes[1]) {
        CNode<T>** q = rep(p);
        (*p)->v = (*q)->v;
        p = q;
    }
    CNode<T>* t = *p;
    *p = (*p)->nodes[(*p)->nodes[1] != nullptr ? 1 : 0];
    delete t;
    calcular_altura(root);
    verificador(root);
    return true;
}
 
// ── rep (reemplazante) ───────────────
template<typename T>
CNode<T>** CBinTree<T>::rep(CNode<T>** p) {
    CNode<T>** q = &((*p)->nodes[!brep]);
    while ((*q)->nodes[brep])
        q = &((*q)->nodes[brep]);
    brep = !brep;
    return q;
}
 
// ── Impresión ──────────────────────
template<typename T>
void CBinTree<T>::print() const {
    inorder(root);
    cout << " h=" << height2(root) << "\n";
}
 
template<typename T>
void CBinTree<T>::inorder(CNode<T>* n) const {
    if (!n) return;
    inorder(n->nodes[0]);
    // Imprimir valor genérico
    if constexpr (is_same_v<T, Fecha>)
        cout << n->v.str() << " ";
    else if constexpr (is_same_v<T, bool>)
        cout << (n->v ? "true" : "false") << " ";
    else
        cout << n->v << " ";
    inorder(n->nodes[1]);
}
 
template<typename T>
int CBinTree<T>::height2(CNode<T>* n) const {
    if (!n) return 0;
    return max(height2(n->nodes[0]), height2(n->nodes[1])) + 1;
}
 
template<typename T>
void CBinTree<T>::print_levels() const {
    cout << "\n";
    levels(root);
    cout << "\n";
}
 
template<typename T>
void CBinTree<T>::levels(CNode<T>* n) const {
    if (!n) return;
    queue<CNode<T>*> q;
    q.push(n);
    while (!q.empty()) {
        CNode<T>* x = q.front(); q.pop();
        if constexpr (is_same_v<T, Fecha>)
            cout << x->v.str() << " ";
        else if constexpr (is_same_v<T, bool>)
            cout << (x->v ? "true" : "false") << " ";
        else
            cout << x->v << " ";
        if (x->nodes[0]) q.push(x->nodes[0]);
        if (x->nodes[1]) q.push(x->nodes[1]);
    }
}
 
// tipo variante que agrupa todos los AVL
using AVLVariant = variant<
    CBinTree<int>,
    CBinTree<float>,
    CBinTree<string>,
    CBinTree<bool>,
    CBinTree<Fecha>
>;
