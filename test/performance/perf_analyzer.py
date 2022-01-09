import pandas as pd
import numpy as np
import sys


if __name__ == "__main__":
    try:
        name_df = sys.argv[1]
    except:
        print("You need to specify the position of the dataset")
        exit()

    df = pd.read_csv(name_df, sep=",")

    df = df[df["Binary Location"] == "writernote"]

    df.drop(["Address", "Source Location", "Binary Location"], inplace=True, axis=1)

    print(df)