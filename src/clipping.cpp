#include "clipping.h"

// Funcao para calcular o Outcode de um ponto
Outcode computeOutcode(double x, double y) {
    // A janela de recorte é X in [-1, 1] e Y in [-1, 1]
    const double XMIN = -1.0;
    const double XMAX = 1.0;
    const double YMIN = -1.0;
    const double YMAX = 1.0;

    Outcode code = INSIDE;

    if (x < XMIN)
        code = (Outcode)(code | LEFT); // 1. Fora à Esquerda
    else if (x > XMAX)
        code = (Outcode)(code | RIGHT); // 2. Fora à Direita

    if (y < YMIN)
        code = (Outcode)(code | BOTTOM); // 3. Fora Abaixo
    else if (y > YMAX)
        code = (Outcode)(code | TOP); // 4. Fora Acima

    return code;
}

bool clipparReta(double& x1, double& y1, double& x2, double& y2) {

    // Os limites do SCN
    const double XMIN = -1.0;
    const double XMAX = 1.0;
    const double YMIN = -1.0;
    const double YMAX = 1.0;

    // Calcula os códigos iniciais
    Outcode code1 = computeOutcode(x1, y1);
    Outcode code2 = computeOutcode(x2, y2);
    bool aceita = false;

    while (true) {
        if (code1 == INSIDE && code2 == INSIDE) {
            // Caso Trivial Aceito: Ambos os pontos estão dentro
            aceita = true;
            break;
        } else if (code1 & code2) {
            // Caso Trivial Rejeitado: Compartilham uma região externa
            // O Bitwise AND é diferente de 0, então a reta pode ser descartada
            break;
        } else {
            // Caso Não-Trivial: A reta cruza a janela. Precisa ser cortada.

            // Escolhe um ponto fora (prioriza o que não está INSIDE)
            Outcode codeFora = (code1 != INSIDE) ? code1 : code2;

            double x_intersecao, y_intersecao;

            // Calcula a inclinação (m) para a equação y = y1 + m(x - x1)
            double m = (y2 - y1) / (x2 - x1);

            // Calcula o ponto de intersecção baseado no código "codeFora"
            if (codeFora & TOP) { // Fora Acima
                x_intersecao = x1 + (YMAX - y1) / m;
                y_intersecao = YMAX;
            } else if (codeFora & BOTTOM) { // Fora Abaixo
                x_intersecao = x1 + (YMIN - y1) / m;
                y_intersecao = YMIN;
            } else if (codeFora & RIGHT) { // Fora à Direita
                y_intersecao = y1 + m * (XMAX - x1);
                x_intersecao = XMAX;
            } else if (codeFora & LEFT) { // Fora à Esquerda
                y_intersecao = y1 + m * (XMIN - x1);
                x_intersecao = XMIN;
            }

            // Move o ponto que está fora para o ponto de intersecção
            if (codeFora == code1) {
                x1 = x_intersecao;
                y1 = y_intersecao;
                code1 = computeOutcode(x1, y1); // Recalcula o código
            } else {
                x2 = x_intersecao;
                y2 = y_intersecao;
                code2 = computeOutcode(x2, y2); // Recalcula o código
            }
        }
    }

    return aceita;
}
