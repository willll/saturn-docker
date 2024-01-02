◎About this sample

  This sample uses the MSB shadow feature of VDP2.

explanation
  MSB Shadow
    MSB shadow looks at the MSB of the framebuffer and the pixel where the MSB stands
    It performs shadow (half-brightness) processing for.
    Normally, on a frame buffer, a pixel with an MSB has a different pixel (with an MSB).
    If you overwrite the pixel data (not standing), the MSB information will disappear.
    Therefore, the screen that appears depends on the order in which the sprites are drawn.
    it's different.
    If you want to set only the MSB on the VDP1 frame buffer, select "MSB On" for VDP1.
    Use the command:
    There are two types of MSB shadows: "sprite shadow" and "transparent shadow".
    there is.
    The conditions under which MSB shadow can be used are as follows.
    ・The frame buffer of VDP1 must be set to 16BIT/DOT.
    ・The sprite type setting of VDP2 must be one of types 2 to 7.
    ・Sprite window enable of VDP2 sprite control register
      Bit setting is disabled (0)
    In addition to the above hardware settings,
    ・The sprite that becomes the shadow has the highest priority.
    There is a condition.
    Effective usage is in vertical scrolling shooting games (aerial perspective).
    Use this when you want to add shadows to both sprite and BG at the same time.
    However, since it takes time to draw normal sprites, if you use it frequently.
    A slowdown in drawing processing occurs.
    If you want to add shadow only to BG, normal shadow is recommended.

  sprite shadow
    This function displays sprites at half brightness.
    The condition is that the MSB is 1 and at least one of the other 15 bits is not 0.
    However, it is a function different from MSB shadow, and it meets the conditions for normal shadow.
    In this case, normal shadow takes precedence.
    A sprite shadow corresponds to the sprite's palette code.
    The color code will be displayed at half brightness.
    It is possible to add a shadow to a specific sprite in the palette code.
    This feature only.
    (Although it is not impossible with color calculations, it is extremely difficult to limit the calculation area.)

  transparent shadow
    This is a function where the BG is displayed at half brightness according to the shape of the sprite in which the MSB is standing.
    The condition is that the MSB is 1 and all other 15 bits are 0.
    The shadow enable bit is set for the dot whose MSB is ON.
    The dot data on the BG surface is displayed at half brightness.
    The shadow enable bit is specified in SGL by the slShadowOn function.


  normal shadow
    Normal shadow is used when adding shadow to BG along the shape of sprite.
    Use.
    The conditions for a sprite to have a normal shadow are as follows.
    ・It must be a pallet code.
    ・Among the palette codes that can be used with sprite types, (maximum value - 1)
      Must be a color code (2047th color if the maximum DOT color code is 2048 colors, etc.)
    There is no need to configure any hardware to use normal shadows.
    (Conversely, it is also said that it is impossible to ban it)
    Sprite data that meets the normal shadow conditions will be transparent.
    You need to be careful about putting it away.
    If the color does not appear in the sprite of the palette code, this may be the cause.
    there is.

  mesh
    Although it has nothing to do with the original shadow function, it can be used to easily create a shadow-like expression.
    I'll list it here because it's often used.
    To use it, simply draw a black sprite using VDP1's mesh drawing.
    There is no need to worry about missing drawings, and no complicated settings are required.
    However, when the horizontal resolution is 320 or 640, rainbow noise is noticeable.
    There are drawbacks.
    If the horizontal resolution is 352 or 704, it will be significantly improved, but it will not disappear completely.
    There isn't.
    If you are aiming to display beautiful shadows, we recommend the shadow function.


that's all.
