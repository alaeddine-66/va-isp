import json
import sys

def charger_modele(path='trigram_model.json'):
    with open(path, 'r') as f:
        modele = json.load(f)
    return modele

def proposer_suggestions(token1, token2, modele):
    cle = f"{token1}->{token2}"
    return modele.get(cle, [])

def main():
    if len(sys.argv) != 3:
        print("Usage : python suggest.py TOKEN1 TOKEN2")
        return

    token1 = sys.argv[1]
    token2 = sys.argv[2]

    modele = charger_modele()
    suggestions = proposer_suggestions(token1, token2, modele)

    if suggestions:
        print(f"💡 Suggestions pour ({token1} {token2}) :")
        for s in suggestions:
            print(f"  → {s}")
    else:
        print(f"❌ Aucune suggestion trouvée pour ({token1} {token2}).")

if __name__ == "__main__":
    main()
