#ifndef CONFIGTOUCHE_H
#define CONFIGTOUCHE_H

/**
 * @brief Active le mode brut du terminal.
 * Désactive le mode canonique et l'affichage des caractères saisis.
 */
void setRawMode();

/**
 * @brief Rétablit le mode par défaut du terminal.
 * Réactive le mode canonique et l'affichage des caractères saisis.
 */
void restoreMode();

#endif // CONFIGTOUCHE_H
