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
import { LAYER_TILES_DEPTH, LAYER_UI_DEPTH, UPDATE_UI_INTERVAL_MS } from '@/game/scenes/Game/const';
import { logPrefixFilename } from '@/helpers/vite';

import { Scene } from 'phaser';

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
    layerUI: Phaser.GameObjects.Layer;
    layerTiles: Phaser.GameObjects.Layer;
    piglet: Phaser.GameObjects.Sprite;
    fpsText: Phaser.GameObjects.Text;
    timerUpdateUI: Phaser.Time.TimerEvent;

    keyW?: Phaser.Input.Keyboard.Key;
    keyA?: Phaser.Input.Keyboard.Key;
    keyS?: Phaser.Input.Keyboard.Key;
    keyD?: Phaser.Input.Keyboard.Key;

    keyUp?: Phaser.Input.Keyboard.Key;
    keyDown?: Phaser.Input.Keyboard.Key;
    keyRight?: Phaser.Input.Keyboard.Key;
    keyBottom?: Phaser.Input.Keyboard.Key;

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
        this.createAnimations();
        this.createUI();
        this.createGameObjects();

        // this.layerUI.depth;

        this.input.once('pointerdown', () => {
            this.piglet.play(ASSET_PIGLET_ANIMATION_WALK, true);

            this.input.once('pointerdown', () => {
                this.scene.start('GameOver');
            });
        });

        this.createInputEvents();

        this.timerUpdateUI = this.time.addEvent({
            delay: UPDATE_UI_INTERVAL_MS,
            callback: () => {
                this.fpsText.text = formatFps(this.game.loop.actualFps);

                // if ((this.input.keyboard as any)?.[Phaser.Input.Keyboard.KeyCodes.W]) {
                //     console.log(`${logPrefixFilename(import.meta.url)}:`, 'W key pressed');
                // }
                if (this.keyW?.isDown) {
                    console.log(`${logPrefixFilename(import.meta.url)}:`, 'W key pressed');
                }

                if (this.keyUp?.isDown) {
                    console.log(`${logPrefixFilename(import.meta.url)}:`, 'Up key pressed');
                }
            },
            loop: true,
        });
    }

    createUI() {
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

        this.layerUI = this.add.layer([this.fpsText]);
        this.layerUI.depth = LAYER_UI_DEPTH;
        // this.layerUI.add([this.fpsText]);
    }

    createAnimations() {
        this.anims.create({
            key: ASSET_PIGLET_ANIMATION_WALK,
            frames: this.anims.generateFrameNumbers(ASSET_PIGLET, {
                start: ASSET_PIGLET_ANIMATION_WALK_FRAMES_START,
                end: ASSET_PIGLET_ANIMATION_WALK_FRAMES_END,
            }),
            frameRate: ASSET_PIGLET_ANIMATION_WALK_FPS,
            repeat: -1,
        });
    }

    createGameObjects() {
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

        this.layerTiles = this.add.layer([this.background, this.piglet]);
        this.layerTiles.depth = LAYER_TILES_DEPTH;
        // this.layerTiles.add([this.background, this.piglet]);
    }

    createInputEvents() {
        this.keyW = this.input.keyboard?.addKey(
            Phaser.Input.Keyboard.KeyCodes.W,
        );

        this.keyUp = this.input.keyboard?.addKey(
            Phaser.Input.Keyboard.KeyCodes.UP,
        );
    }

    update(time: number, delta: number): void {
    }
}
