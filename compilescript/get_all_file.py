import os
import sys

def posizione(stringa="") -> str:
    """
    :parameter stringa: se è diversa da "" allora fa il return di se stessa [usata per debug]
    :return: stringa -> ritorna la posizione inserita dall'utente
    """

    if stringa != "":
        return stringa

    print("Inserisci la posizione del path")
    stringa = input(stringa)
    return stringa


def linee_count(path: str, name: str) -> int:
    """
    :param path: posizione assoluta del file
    :return: fa il return della lunghezza dei file
    """
    i = 0
    with open(path + "/" + name, "r") as file:
        for i, _ in enumerate(file):
            pass

    return i

def func(show: bool, ramo: str, dir:str) -> int:
    if show:
        print("Open file " + ramo + "/" + dir)

    return linee_count(ramo, dir)

def ricorsione(lista: list, ramo: str, show = False) -> list[str]:
    """
    :param lista: lista -> tutte le cartelle e file nella cartella in cui siamo
    :param num: aggiunge a num il valore delle linee totali
    :param: ramo: indice alla funzione dove si trova nell'albero della funzione
    :return:
    """

    if show:
        print("Ramo " + ramo)

    lista_sec = []


    for dir in lista:
        if ".cpp" in dir:
            lista_sec.append(ramo + "/" + dir)

        elif not "." in dir:
            temp_dir = ramo + "/" + dir

            try:
                lista_sottocartella = os.listdir(temp_dir)
            except NotADirectoryError:
                print("Not a directory")
                continue

            tmp = ricorsione(lista_sottocartella, temp_dir)

            for x in tmp:
                lista_sec.append(x)


    return lista_sec


def main(stringa: str, show: bool) -> None:
    """
    :param stringa: indice la posizione in cui è il progetto
    :param show: true show all fine output
    :return:
    """

    os.path.join(stringa)
    lista = os.listdir(stringa)

    tmp = ricorsione(lista, stringa, show)
    for x in tmp:
        print(x)

if __name__ == "__main__":
    main("/home/giacomo/writernote-qt/src", True)