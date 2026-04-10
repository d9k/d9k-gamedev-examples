import { Scene, GameObjects } from 'phaser';
import { GAME_TITLE } from '~/const/main';

export class MainMenu extends Scene
{
    background: GameObjects.Image;
    logo: GameObjects.Image;
    sceneTitle: GameObjects.Text;
    gameTitle: GameObjects.Text;

    constructor ()
    {
        super('MainMenu');
    }

    create ()
    {
        this.background = this.add.image(512, 384, 'background');

        this.logo = this.add.image(512, 300, 'logo');

        const textStyle =  {
            fontFamily: 'Arial Black',
            fontSize: 38,
            color: '#ffffff',
            stroke: '#000000',
            strokeThickness: 8,
            align: 'center'
        }

        this.gameTitle = this.add.text(512, 420, GAME_TITLE, {...textStyle, color: '#ffff00', stroke: '#222222'}).setOrigin(0.5);
        this.sceneTitle = this.add.text(512, 500, 'Main Menu', textStyle).setOrigin(0.5);

        this.input.once('pointerdown', () => {

            this.scene.start('Game');

        });
    }
}
