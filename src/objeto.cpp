#include "objeto.h"

Objeto::Objeto(string nome, tipoObjeto tipo) {
    this->nome = nome;
    this->tipo = tipo;
}

string Objeto::getNome(){
    return this->nome;
}

Objeto::~Objeto() {}
