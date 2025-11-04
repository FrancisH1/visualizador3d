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
    void adicionarObjetos(std::vector<std::unique_ptr<Objeto>> novosObjetos);
    const std::vector<std::unique_ptr<Objeto>>& getObjetos() const { return objetos; }

    void removerObjeto(string nome);

    void aplicarTransformacao(const Matriz4x4& M);

    ~DisplayFile();
};

#endif // DISPLAYFILE_H
