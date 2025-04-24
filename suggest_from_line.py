import sys
import json

def charger_modele(path='trigram_model.json'):
    with open(path, 'r') as f:
        return json.load(f)

def tokenize(code):
    tokens = []
    token = ''
    for c in code:
        if c in '()':
            if token:
                tokens.append(token)
                token = ''
            tokens.append(c)
        elif c.isspace():
            if token:
                tokens.append(token)
                token = ''
        else:
            token += c
    if token:
        tokens.append(token)
    return tokens

def main():
    if len(sys.argv) != 2:
        print("")
        return

    ligne = sys.argv[1]
    tokens = tokenize(ligne)
    if len(tokens) < 2:
        print("")
        return

    t1, t2 = tokens[-2], tokens[-1]
    modele = charger_modele()
    key = f"{t1}->{t2}"
    suggestions = modele.get(key, [])
    print(" ".join(suggestions))

if __name__ == "__main__":
    main()
