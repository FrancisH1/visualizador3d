#include "window3d.h"

Window3d::Window3d(const Ponto& eye, const Ponto& lookAt, const Ponto& vup,
             double xMin, double xMax,
             double yMin, double yMax,
             double zMin, double zMax)
        : eye(eye), lookAt(lookAt), vup(vup),
          xMin(xMin), xMax(xMax),
          yMin(yMin), yMax(yMax),
          zMin(zMin), zMax(zMax) {}

Ponto Window3d::getEye() const { return eye; }

Ponto Window3d::getLookAt() const { return lookAt; }

Ponto Window3d::getVup() const { return vup; }

Matriz4x4 Window3d::getMatrizNormalizacao() const {
    // Vetor n = direção do olhar (para frente)
    Ponto n = (eye - lookAt).normalizado();

    // Vetor u = eixo "direita" da câmera (vup × n)
    Ponto u = vup.produtoVetorial(n).normalizado();

    // Vetor v = eixo "para cima" (n × u)
    Ponto v = n.produtoVetorial(u);

    // Matriz de rotação (alinha o mundo ao sistema da câmera)
    Matriz4x4 R({
        u.x(), u.y(), u.z(), 0,
        v.x(), v.y(), v.z(), 0,
        n.x(), n.y(), n.z(), 0,
        0,     0,     0,     1
    });

    // Matriz de translação (leva a câmera para origem)
    Matriz4x4 T = Matriz4x4::translacao(-eye.x(), -eye.y(), -eye.z());

    // Matriz final (rotação * translação)
    return R * T;
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
    eye.coordenada[0] += dx;
    eye.coordenada[1] += dy;
    eye.coordenada[2] += dz;
    lookAt.coordenada[0] += dx;
    lookAt.coordenada[1] += dy;
    lookAt.coordenada[2] += dz;
}

void Window3d::rotacionarY(double anguloGraus) {
    double ang = anguloGraus * M_PI / 180.0;
    Ponto direcao = (lookAt - eye);
    double x = direcao.coordenada[0];
    double z = direcao.coordenada[2];

    direcao.coordenada[0] = x * cos(ang) + z * sin(ang);
    direcao.coordenada[2] = -x * sin(ang) + z * cos(ang);

    lookAt = eye + direcao;
}

void Window3d::rotacionarX(double anguloGraus) {
    double rad = anguloGraus * M_PI / 180.0;

    // Direção da visão (lookAt - eye)
    Ponto dir = lookAt - eye;

    // Rotação em torno do eixo X
    double y = dir.y() * cos(rad) - dir.z() * sin(rad);
    double z = dir.y() * sin(rad) + dir.z() * cos(rad);

    dir.coordenada[1] = y;
    dir.coordenada[2] = z;

    // Atualiza lookAt e vup
    lookAt = eye + dir;

    // Também rotaciona o vetor VUP (mantém "para cima" relativo à câmera)
    double vy = vup.y() * cos(rad) - vup.z() * sin(rad);
    double vz = vup.y() * sin(rad) + vup.z() * cos(rad);
    vup.coordenada[1] = vy;
    vup.coordenada[2] = vz;
}

void Window3d::escalar(double fator) {
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