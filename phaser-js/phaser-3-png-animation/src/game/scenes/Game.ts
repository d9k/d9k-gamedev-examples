import { FONT_SIZE_SMALL, GAME_WIDTH_CENTER } from '@/game/const/main';
import { Scene } from 'phaser';

export class Game extends Scene
{
    camera: Phaser.Cameras.Scene2D.Camera;
    background: Phaser.GameObjects.Image;
    msg_text : Phaser.GameObjects.Text;

    constructor ()
    {
        super('Game');
    }

    create ()
    {
        this.camera = this.cameras.main;
        this.camera.setBackgroundColor(0x00ff00);

        this.background = this.add.image(GAME_WIDTH_CENTER, 0, 'background');
        this.background.setAlpha(0.5);

        this.msg_text = this.add.text(GAME_WIDTH_CENTER, 4, 'Make something fun!\nand share it with us:\nsupport@phaser.io', {
            fontFamily: 'Arial Black', fontSize: FONT_SIZE_SMALL, color: '#ffffff',
            stroke: '#000000', strokeThickness: 3,
            align: 'center'
        }).setOrigin(0.5, 0);

        this.input.once('pointerdown', () => {

            this.scene.start('GameOver');

        });
    }
}
