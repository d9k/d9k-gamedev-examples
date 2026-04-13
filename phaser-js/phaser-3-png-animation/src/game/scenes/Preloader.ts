import { GAME_HEIGHT_CENTER, GAME_WIDTH, GAME_WIDTH_CENTER } from '@/game/const/main';
import { Scene } from 'phaser';

const __filename = (() => {
    const name = import.meta.url.substring(import.meta.url.lastIndexOf('/') + 1);
    const qIndex = name.lastIndexOf('?');
    return qIndex !== -1 ? name.substring(0, qIndex) : name;
})();

export class Preloader extends Scene
{
    constructor ()
    {
        super('Preloader');
    }

    init ()
    {
        //  We loaded this image in our Boot Scene, so we can display it here
        this.add.image(GAME_WIDTH_CENTER, GAME_HEIGHT_CENTER, 'background');

        //  A simple progress bar. This is the outline of the bar.
        this.add.rectangle(GAME_WIDTH_CENTER, GAME_HEIGHT_CENTER, GAME_WIDTH - 40, 32).setStrokeStyle(1, 0xffffff);

        //  This is the progress bar itself. It will increase in size from the left based on the % of progress.
        const bar = this.add.rectangle(20, GAME_HEIGHT_CENTER, 4, 28, 0xffffff).setOrigin(0, 0.5);

        //  Use the 'progress' event emitted by the LoaderPlugin to update the loading bar
        this.load.on('progress', (progress: number) => {

            //  Update the progress bar (our bar is 464px wide, so 100% = 464px)
            bar.width = 4 + ((GAME_WIDTH_CENTER - 40) * progress);

            console.log(__filename, ': progress:', progress);

        });
    }

    preload ()
    {
        //  Load the assets for the game - Replace with your own assets
        this.load.setPath('assets');

        this.load.image('logo', 'logo.png');
    }

    create ()
    {
        //  When all the assets have loaded, it's often worth creating global objects here that the rest of the game can use.
        //  For example, you can define global animations here, so we can use them in other scenes.

        //  Move to the MainMenu. You could also swap this for a Scene Transition, such as a camera fade.
        this.scene.start('MainMenu');
    }
}
