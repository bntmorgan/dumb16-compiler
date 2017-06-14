/*
Copyright (C) 2017  Benoît Morgan

This file is part of dumb16-compiler.

dumb16-compiler is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

dumb16-compiler is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with dumb16-compiler.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __OPT_H__
#define __OPT_H__

/**
 * Fichier de sortie
 *
 * Si on ne spécifie pas de fichier avec l'option o 
 * a.out.s sera choisi
 */
extern FILE* file_out;

/**
 * Traite les options du compilateur
 *
 * @param argc Nombre d'argument y compris le nom de l'exécutable
 * @param argv Tableau des arguments
 */
void do_options(int argc, char **argv);

/**
 * Fermeture des fichiers et annulation de redirection de l'entrée standard
 */
void close_files();

extern FILE *stdin_terminal;

#endif//__OPT_H__
