import {
    ASSET_PIGLET,
    ASSET_PIGLET_ANIMATION_WALK,
    ASSET_PIGLET_ANIMATION_WALK_FPS,
    ASSET_PIGLET_ANIMATION_WALK_FRAMES_END,
    ASSET_PIGLET_ANIMATION_WALK_FRAMES_START,
    ASSET_PIGLET_FRAME_SIDE_PX,
    ASSET_PIGLET_PATH,
} from '@/game/const/assets';

import {
    FONT_SIZE_SMALL,
    GAME_HEIGHT_CENTER,
    GAME_WIDTH_CENTER,
} from '@/game/const/main';

import { Scene } from 'phaser';

const UPDATE_UI_INTERVAL_MS = 500;

function formatFps(fps?: number) {
    let fpsText = '?';
    if (fps) {
        fpsText = `${Math.floor(fps)}`;
    }
    return `FPS: ${fpsText}`;
}

export class Game extends Scene {
    camera: Phaser.Cameras.Scene2D.Camera;
    background: Phaser.GameObjects.Image;
    piglet: Phaser.GameObjects.Sprite;
    fpsText: Phaser.GameObjects.Text;
    timerUpdateUI: Phaser.Time.TimerEvent;

    constructor() {
        super('Game');
    }

    preload() {
        this.load.spritesheet(ASSET_PIGLET, ASSET_PIGLET_PATH, {
            frameWidth: ASSET_PIGLET_FRAME_SIDE_PX,
            frameHeight: ASSET_PIGLET_FRAME_SIDE_PX,
        });
    }

    create() {
        this.camera = this.cameras.main;
        this.camera.setBackgroundColor(0x00ff00);

        this.background = this.add.image(GAME_WIDTH_CENTER, 0, 'background');
        this.background.setAlpha(0.5);

        this.piglet = this.add.sprite(
            GAME_WIDTH_CENTER,
            GAME_HEIGHT_CENTER,
            ASSET_PIGLET,
            0
        );

        const defaultTextStyle: Phaser.Types.GameObjects.Text.TextStyle = {
            fontFamily: 'Arial Black',
            fontSize: FONT_SIZE_SMALL,
            color: '#ffffff',
            stroke: '#000000',
            strokeThickness: 3,
            align: 'center',
        };

        this.fpsText = this.add
            .text(8, 4, formatFps(), defaultTextStyle)
            .setOrigin(0, 0);

        this.timerUpdateUI = this.time.addEvent({
            delay: UPDATE_UI_INTERVAL_MS, // ms
            callback: () => {
                this.fpsText.text = formatFps(this.game.loop.actualFps);
            },
            //args: [],
            loop: true,
        });

        this.anims.create({
            key: ASSET_PIGLET_ANIMATION_WALK,
            frames: this.anims.generateFrameNumbers(ASSET_PIGLET, {
                start: ASSET_PIGLET_ANIMATION_WALK_FRAMES_START,
                end: ASSET_PIGLET_ANIMATION_WALK_FRAMES_END,
            }),
            frameRate: ASSET_PIGLET_ANIMATION_WALK_FPS,
            repeat: -1,
        });

        this.input.once('pointerdown', () => {
            this.piglet.play(ASSET_PIGLET_ANIMATION_WALK, true);

            this.input.once('pointerdown', () => {
                this.scene.start('GameOver');
            });
        });
    }
}
