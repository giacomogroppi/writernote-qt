import os
import sys

def ricorsione(lista: list, ramo: str) -> list[str]:
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


def main(stringa: str) -> None:
    os.path.join(stringa)
    lista = os.listdir(stringa)

    tmp = ricorsione(lista, stringa)
    for x in tmp:
        print(x)

if __name__ == "__main__":
    main(sys.argv[1])