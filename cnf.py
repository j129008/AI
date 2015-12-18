
ops = { 'iff': 1, 'imp': 2, 'or': 3, 'and': 4, 'not': 5 }
opStack = []
postfix = []

text = raw_input("input your logic: ")
tokens = text.split(" ")
print tokens

while len(tokens) > 0 :
    token = tokens.pop(0)
    if token not in ops :  # check op or symbol or ( or )
        if token == '(':
            opStack = list(token) + opStack
        elif token == ')':
            while opStack[0] != '(' :
                postfix.append(opStack.pop(0))
            opStack.pop(0)
        else:
            postfix.append(token)
    elif len( opStack ) == 0 : # check op stack empty
        opStack.append(token)
    else:
        while ops[token] <= ops[opStack[0]]:
            postfix.append(opStack.pop(0))
            if len(opStack) == 0 :
                break
        opStack = [token] + opStack


while len(opStack) > 0 :
    postfix.append(opStack.pop(0))

print postfix
