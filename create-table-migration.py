#!venv/bin/python
import pygmalion.models

print('"""\nCreating base tables\n"""\n')

print("from yoyo import step\n\nsteps = [\n")
for i in pygmalion.models.modules:
    tables = pygmalion.models.create_table(i)
    for up, down in tables:
        if '\n' in up:
            print('    step("""')
            for line in up.split("\n"):
                line.strip()
                print('         {}'.format(line))
            print('         """,')
        else:
            print('    step("""{}""",\n'.format(up))
        if '\n' in down:
            print('         """')
            for line in down.split("\n"):
                line.strip()
                print('         {}'.format(line))
            print('         """\n    ),')
        else:
            print('         """{}"""\n    ),'.format(down))
print("]")
