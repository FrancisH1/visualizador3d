#include "displayfile.h"

DisplayFile::DisplayFile() {}

void DisplayFile::adicionarObjeto(unique_ptr<Objeto> obj){
    objetos.push_back(std::move(obj));
}

void DisplayFile::adicionarObjetos(std::vector<std::unique_ptr<Objeto>> novosObjetos) {
    for (auto& obj : novosObjetos)
        objetos.push_back(std::move(obj));
}

void DisplayFile::removerObjeto(string nome){
    for (auto it = objetos.begin(); it != objetos.end(); ) {
        // Acessamos o objeto através do ponteiro inteligente (*it)
        if ( (*it)->nome == nome ) { // Assumindo que 'nome' é um atributo acessível
            // Remove o elemento no iterador 'it' e o erase retorna o iterador para o próximo elemento válido
            it = objetos.erase(it);
            // Não precisa fazer 'it++' aqui, pois o erase já nos deu o próximo
            // Como é para remover *um* objeto por nome, podemos parar.
            return;
        } else {
            // Se não remover, avança para o próximo
            ++it;
        }
    }
}

void DisplayFile::aplicarTransformacao(const Matriz4x4 &M){
    for(auto& obj : this->objetos){
        (*obj).aplicarTransformacao(M);
    }
}

DisplayFile::~DisplayFile() {}
