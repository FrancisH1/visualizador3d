#include "window3d.h"

Window3d::Window3d(const Ponto& eye, const Ponto& lookAt, const Ponto& vup,
             double xMin, double xMax,
             double yMin, double yMax,
             double zMin, double zMax)
        : eye(eye), lookAt(lookAt), vup(vup),
          xMin(xMin), xMax(xMax),
          yMin(yMin), yMax(yMax),
          zMin(zMin), zMax(zMax) 
        {
            this->fov = 60.0;
            this->near = 0.1;
            this->far = 1000.0;
        }

Ponto Window3d::getEye() const { return eye; }

Ponto Window3d::getLookAt() const { return lookAt; }

Ponto Window3d::getVup() const { return vup; }

Matriz4x4 Window3d::getMatrizView() const
{
    // 1. forward = direção para onde a câmera olha
    Ponto f = (lookAt - eye).normalizado();

    // 2. right = perpendicular (lado direito da câmera)
    Ponto u = f.produtoVetorial(vup).normalizado();

    // 3. up corrigido
    Ponto v = u.produtoVetorial(f);

    // 4. Matriz de rotação
    Matriz4x4 R({
        u.x(),  u.y(),  u.z(),  0,
        v.x(),  v.y(),  v.z(),  0,
       -f.x(), -f.y(), -f.z(),  0,
         0,      0,      0,     1
    });

    // 5. Matriz de translação
    Matriz4x4 T = Matriz4x4::translacao(-eye.x(), -eye.y(), -eye.z());

    return R * T;
}

Matriz4x4 Window3d::getMatrizPerspectiva(double aspect) const {
    // fov em graus -> rad
    double fovRad = (fov * M_PI) / 180.0;
    double f = 1.0 / std::tan(fovRad / 2.0); // cot(fov/2)

    return Matriz4x4({
        f / aspect,     0.0,            0.0,                     0.0,
            0.0,        f,              0.0,                     0.0,
            0.0,        0.0, (far + near) / (near - far), (2.0 * far * near) / (near - far),
            0.0,        0.0,            -1.0,                    0.0
    });
}

Matriz4x4 Window3d::getMatrizProjecaoOrtogonal() const {
    double sx = 2.0 / (xMax - xMin);
    double sy = 2.0 / (yMax - yMin);
    double sz = 2.0 / (zMax - zMin);

    double tx = -(xMax + xMin) / (xMax - xMin);
    double ty = -(yMax + yMin) / (yMax - yMin);
    double tz = -(zMax + zMin) / (zMax - zMin);

    return Matriz4x4({
        sx, 0,  0,  tx,
        0,  sy, 0,  ty,
        0,  0,  sz, tz,
        0,  0,  0,  1
    });
}

void Window3d::transladar(double dx, double dy, double dz) {
    // Movimento da câmera em 3D
    eye.coordenada[0] += dx;
    eye.coordenada[1] += dy;
    eye.coordenada[2] += dz;

    lookAt.coordenada[0] += dx;
    lookAt.coordenada[1] += dy;
    lookAt.coordenada[2] += dz;
}


void Window3d::rotacionarY(double anguloGraus) {
    double rad = anguloGraus * M_PI / 180.0;

    Ponto f = (lookAt - eye).normalizado();
    Ponto up = vup.normalizado();
    Ponto right = f.produtoVetorial(up).normalizado();

    double cosA = cos(rad);
    double sinA = sin(rad);

    Ponto novoF(
        f.x() * cosA + right.x() * sinA,
        f.y() * cosA + right.y() * sinA,
        f.z() * cosA + right.z() * sinA,
        0.0
    );

    lookAt = eye + novoF;
}

void Window3d::rotacionarX(double anguloGraus) {
    double rad = anguloGraus * M_PI / 180.0;

    Ponto f = (lookAt - eye).normalizado();
    Ponto up = vup.normalizado();
    Ponto right = f.produtoVetorial(up).normalizado();

    double cosA = cos(rad);
    double sinA = sin(rad);

    Ponto novoF(
        f.x() * cosA + up.x() * sinA,
        f.y() * cosA + up.y() * sinA,
        f.z() * cosA + up.z() * sinA,
        0.0
    );

    if (novoF.y() > 0.98 || novoF.y() < -0.98) return;

    lookAt = eye + novoF;
    this->vup = right.produtoVetorial(novoF).normalizado();
}

void Window3d::escalar(double fator, tipoProjecao projecaoAtual) {

    if(projecaoAtual == ORTOGONAL){
        double cx = (xMin + xMax) / 2.0;
        double cy = (yMin + yMax) / 2.0;
        double cz = (zMin + zMax) / 2.0;

        double largura = (xMax - xMin) * fator / 2.0;
        double altura  = (yMax - yMin) * fator / 2.0;
        double profundidade = (zMax - zMin) * fator / 2.0;

        xMin = cx - largura;  xMax = cx + largura;
        yMin = cy - altura;   yMax = cy + altura;
        zMin = cz - profundidade; zMax = cz + profundidade;
    }
    else{
        // Zoom por FOV
        fov *= fator;

        // trava FOV
        if (fov < 10.0) fov = 10.0;
        if (fov > 120.0) fov = 120.0;
    }
}

void Window3d::resetarCamera()
{
    eye = Ponto(0, 0, 5, 1);
    lookAt = Ponto(0, 0, 0, 1);
    vup = Ponto(0, 1, 0, 0);

    fov = 60.0;
}