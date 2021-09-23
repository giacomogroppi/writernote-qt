import os


def ricorsione(lista: list, ramo: str, show = False) -> list[str]:
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