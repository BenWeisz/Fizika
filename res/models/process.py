with open("./ball.obj", "r") as in_file:
    lines = in_file.readlines()
    lines = [line.strip() for line in lines]
    lines = list(filter(lambda x: len(x) > 0 and (x[:2] == "v " or x[0] == "f"), lines))

    def facemap(x):
        if x[0] == "v":
            return x
        else:
            split = x.split(" ")[1:]
            split = [token.split("/")[0] for token in split]
            return "f " + " ".join(split)

    lines = list(map(facemap, lines))
    with open("ball2.obj", "w") as out_file:
        for x in lines:
            out_file.write(x + "\n")