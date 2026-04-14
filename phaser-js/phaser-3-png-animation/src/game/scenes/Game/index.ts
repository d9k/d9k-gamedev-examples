import {
    ASSET_PIGLET,
    ASSET_PIGLET_ANIMATION_WALK,
    ASSET_PIGLET_ANIMATION_WALK_FPS,
    ASSET_PIGLET_ANIMATION_WALK_FRAMES_END,
    ASSET_PIGLET_ANIMATION_WALK_FRAMES_START,
    ASSET_PIGLET_FRAME_SIDE_PX,
    ASSET_PIGLET_PATH,
} from '@/game/const/assets';
import { DELTA, PIGLET_SPEED_CELLS_BY_S, PX_IN_CELL } from '@/game/const/gamePhysics';

import {
    FONT_SIZE_SMALL,
    GAME_HEIGHT_CENTER,
    GAME_WIDTH_CENTER,
} from '@/game/const/main';
import {
    LAYER_TILES_DEPTH,
    LAYER_UI_DEPTH,
    UPDATE_UI_INTERVAL_MS,
} from '@/game/scenes/Game/const';
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

    moveDirectionVector?: Phaser.Math.Vector2;

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

        this.input.once('pointerdown', () => {
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
                    console.log(
                        `${logPrefixFilename(import.meta.url)}:`,
                        'W key pressed'
                    );
                }

                if (this.keyUp?.isDown) {
                    console.log(
                        `${logPrefixFilename(import.meta.url)}:`,
                        'Up key pressed'
                    );
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
    }

    createInputEvents() {
        // WASD keys
        this.keyW = this.input.keyboard?.addKey(
            Phaser.Input.Keyboard.KeyCodes.W
        );
        this.keyA = this.input.keyboard?.addKey(
            Phaser.Input.Keyboard.KeyCodes.A
        );
        this.keyS = this.input.keyboard?.addKey(
            Phaser.Input.Keyboard.KeyCodes.S
        );
        this.keyD = this.input.keyboard?.addKey(
            Phaser.Input.Keyboard.KeyCodes.D
        );

        // Arrow keys
        this.keyUp = this.input.keyboard?.addKey(
            Phaser.Input.Keyboard.KeyCodes.UP
        );
        this.keyDown = this.input.keyboard?.addKey(
            Phaser.Input.Keyboard.KeyCodes.DOWN
        );
        this.keyRight = this.input.keyboard?.addKey(
            Phaser.Input.Keyboard.KeyCodes.RIGHT
        );
        this.keyBottom = this.input.keyboard?.addKey(
            Phaser.Input.Keyboard.KeyCodes.LEFT
        );
    }

    update(currentTime: number, deltaMs: number): void {
        this.moveDirectionVector = new Phaser.Math.Vector2(0, 0);

        // Horizontal movement (D/Right and A/Left)
        if (this.keyRight?.isDown || this.keyD?.isDown) {
            this.moveDirectionVector.x = 1;
        }
        if (this.keyBottom?.isDown || this.keyA?.isDown) {
            this.moveDirectionVector.x = -1;
        }

        // Vertical movement (W/Up and S/Down)
        if (this.keyUp?.isDown || this.keyW?.isDown) {
            this.moveDirectionVector.y = -1;
        }
        if (this.keyDown?.isDown || this.keyS?.isDown) {
            this.moveDirectionVector.y = 1;
        }

        this.moveDirectionVector.normalize();

        if (this.moveDirectionVector.length() > DELTA) {
            const moveVector = this.moveDirectionVector
                .clone()
                .scale((PIGLET_SPEED_CELLS_BY_S * PX_IN_CELL * deltaMs) / 1000);

            this.piglet.x += moveVector.x;
            this.piglet.y += moveVector.y;

            if (!this.piglet.anims.hasStarted) {
                this.piglet.play(ASSET_PIGLET_ANIMATION_WALK, true);
            } else {
                this.piglet.anims.resume();
            }
        } else {
            this.piglet.anims.pause();
        }

    }
}
