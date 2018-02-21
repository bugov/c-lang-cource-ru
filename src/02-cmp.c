int main (int argc, char *argv[])
{
    if (argc != 3) {
        return 1;
    }

    char a;
    char b;
    for (int count = 1; count < argc; count++) {
        if (count == 1) {
            a = argv[count][0];
        }

        if (count == 2) {
            b = argv[count][0];
        }
    }

    if (a < b) {
        return 1;
    }
    else if (a == b) {
        return 0;
    }
    else {
        return 2;
    }
}

