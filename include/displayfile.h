#ifndef DISPLAYFILE_H
#define DISPLAYFILE_H

#include <vector>
#include <memory>
#include "objeto.h"

using std::unique_ptr;

class DisplayFile
{
public:
    vector<unique_ptr<Objeto>> objetos;

    DisplayFile();

    void adicionarObjeto(unique_ptr<Objeto> obj);

    void removerObjeto(string nome);

    void aplicarTransformacao(const Matriz3x3& M);

};

#endif // DISPLAYFILE_H
