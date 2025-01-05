TEXT = '''	Sprite {}
		Transform
			position: ({}, {}, 0)
		Sprite
			sprite: def0000000000019
			color: White
		Enabled
		Renderable'''

SIZE = 10

index = 0
for y in range(SIZE):
    for x in range(SIZE):
        print(TEXT.format(index, x, y))
        index += 1