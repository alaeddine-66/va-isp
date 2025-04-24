import json
import sys
from collections import defaultdict

def tokenize_lisp(code):
    """
    Très simple tokenizer Lisp basé sur les espaces et les parenthèses.
    """
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

def build_trigram_model(tokens):
    """
    Construit un dictionnaire : (a, b) → [c1, c2, ...]
    """
    model = defaultdict(list)
    for i in range(len(tokens) - 2):
        key = (tokens[i], tokens[i + 1])
        next_token = tokens[i + 2]
        model[key].append(next_token)
    return model

def main():
    if len(sys.argv) != 2:
        print("Usage : python train_trigram_model.py fichier.lisp")
        return

    path = sys.argv[1]
    with open(path, 'r') as f:
        code = f.read()

    tokens = tokenize_lisp(code)
    model = build_trigram_model(tokens)

    # Convert to JSON-serializable format
    json_model = {f"{a}->{b}": cs for (a, b), cs in model.items()}

    with open("trigram_model.json", "w") as f:
        json.dump(json_model, f, indent=2)

    print("✅ Modèle trigramme enregistré dans 'trigram_model.json'.")

if __name__ == "__main__":
    main()
