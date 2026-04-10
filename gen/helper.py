def Indent(text, indent=1):
    if text is None:
        return ''
    res = ''
    strip = False
    if text[-1] != '\n':
        text += '\n'
        strip = True
    lines = text.split('\n')[:-1]
    if len(lines) == 0:
        return '  ' * indent + text
    for l in lines:
        if l == '':
            res += '\n'
            continue
        res += '  ' * indent + l + '\n'
    if strip and res[-1] == '\n':
        return res[:-1]
    return res
